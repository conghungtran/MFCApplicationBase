#include "pch.h"
#include "ImportExportService.h"
#include "DatabaseManager.h"

CImportExportService::CImportExportService(std::shared_ptr<IBookRepository> repository)
    : m_repository(repository)
{
}

std::vector<CString> CImportExportService::SplitCsvLine(const CString& line)
{
    std::vector<CString> tokens;
    CString remaining = line;
    int pos;

    while ((pos = remaining.Find(_T(','))) != -1)
    {
        tokens.push_back(remaining.Left(pos).Trim());
        remaining = remaining.Mid(pos + 1);
    }
    tokens.push_back(remaining.Trim());

    return tokens;
}

ImportResult CImportExportService::ImportFromCSV(const CString& filePath)
{
    ImportResult result;

    CStdioFile file;
    CFileException fe;
    if (!file.Open(filePath, CFile::modeRead | CFile::typeText, &fe))
    {
        result.errorMsg = _T("Không thể mở file CSV.");
        return result;
    }

    CDatabase& db = CDatabaseManager::Instance().GetDatabase();
    CString line;
    bool firstLine = true;

    TRY
    {
        db.BeginTrans();

        while (file.ReadString(line))
        {
            if (firstLine)          // dòng header NAME,PRICE,QTY -> bỏ qua
            {
                firstLine = false;
                continue;
            }
            if (line.Trim().IsEmpty())
                continue;

            result.totalRows++;

            std::vector<CString> tokens = SplitCsvLine(line);
            if (tokens.size() < 3)
            {
                result.failedRows++;
                continue;
            }

            Book book;
            book.Name = tokens[0];
            book.Price = _ttof(tokens[1]);
            book.Qty = _ttoi(tokens[2]);

            if (book.Name.IsEmpty() || book.Price <= 0 || book.Qty < 0)
            {
                result.failedRows++;
                continue;
            }

            if (m_repository->Add(book))
                result.successRows++;
            else
                result.failedRows++;
        }

        db.CommitTrans();
    }
        CATCH(CDBException, e)
    {
        db.Rollback();
        result.errorMsg.Format(_T("Import thất bại, đã rollback: %s"), e->m_strError);
    }
    END_CATCH

        file.Close();
    return result;
}

bool CImportExportService::ExportToCSV(const CString& filePath, CString& errorMsg)
{
    std::vector<Book> books = m_repository->GetAll();

    CStdioFile file;
    CFileException fe;
    if (!file.Open(filePath, CFile::modeCreate | CFile::modeWrite | CFile::typeText, &fe))
    {
        errorMsg = _T("Không thể tạo file export.");
        return false;
    }

    file.WriteString(_T("NAME,PRICE,QTY\n")); //     file.WriteString(_T("ID,NAME,PRICE,QTY\n"));

    for (const auto& book : books)
    {
        CString line;
        line.Format(_T("%s,%.2f,%d\n"), book.Name, book.Price, book.Qty);
        file.WriteString(line);
    }

    file.Close();
    return true;
}

// ---- Async import (multi-thread) ----

struct ImportThreadParam
{
    CImportExportService* service;
    CString filePath;
    void (*onComplete)(const ImportResult&, void* context);
    void* context;
};

static UINT ImportThreadProc(LPVOID pParam)
{
    ImportThreadParam* param = (ImportThreadParam*)pParam;

    ImportResult result = param->service->ImportFromCSV(param->filePath);

    if (param->onComplete)
        param->onComplete(result, param->context);

    delete param;
    return 0;
}

void CImportExportService::ImportFromCSVAsync(const CString& filePath,
    void (*onComplete)(const ImportResult&, void* context), void* context)
{
    ImportThreadParam* param = new ImportThreadParam();
    param->service = this;
    param->filePath = filePath;
    param->onComplete = onComplete;
    param->context = context;

    AfxBeginThread(ImportThreadProc, param);
}