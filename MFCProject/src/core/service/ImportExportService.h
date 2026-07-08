#pragma once

#include <memory>
#include <vector>
#include "IBookRepository.h"
#include "IDatabaseManager.h"

struct ImportResult
{
    int     totalRows = 0;
    int     successRows = 0;
    int     failedRows = 0;
    CString errorMsg;
};

class CImportExportService
{
public:
    explicit CImportExportService(
        std::shared_ptr<IBookRepository> repository,
		IDatabaseManager* databaseManager
    );

    ImportResult ImportFromCSV(const CString& filePath);

    bool ExportToCSV(const CString& filePath, CString& errorMsg);

    void ImportFromCSVAsync(const CString& filePath, void (*onComplete)(const ImportResult&, void* context), void* context);

private:
    std::vector<CString> SplitCsvLine(const CString& line);

    std::shared_ptr<IBookRepository> m_repository;
	IDatabaseManager& m_databaseManager;
};