#pragma once
#include <memory>
#include <vector>
#include "../Repository/IBookRepository.h"

struct ImportResult
{
    int     totalRows = 0;
    int     successRows = 0;
    int     failedRows = 0;
    CString errorMsg;
};

// ==============================
// Service Import / Export CSV
// ==============================
class CImportExportService
{
public:
    explicit CImportExportService(std::shared_ptr<IBookRepository> repository);

    // Import đồng bộ (block UI) - dùng cho file nhỏ, có transaction
    ImportResult ImportFromCSV(const CString& filePath);

    // Export ra file CSV
    bool ExportToCSV(const CString& filePath, CString& errorMsg);

    // Import bất đồng bộ (AfxBeginThread) - dùng cho file 10,000+ dòng
    // onComplete được gọi trên WORKER THREAD, KHÔNG được đụng UI trực tiếp
    // -> phải PostMessage một message tự định nghĩa về dialog chính,
    //    rồi cập nhật UI trong hàm xử lý message đó của dialog.
    void ImportFromCSVAsync(const CString& filePath, void (*onComplete)(const ImportResult&, void* context), void* context);

private:
    std::vector<CString> SplitCsvLine(const CString& line);

    std::shared_ptr<IBookRepository> m_repository;
};