#pragma once
#include <sql.h>
#include <sqlext.h>

enum
{
	WVARCHAR_MAX = 4000,
	BINARY_MAX = 8000
};

class DBConnection
{
public:
	bool			Connect(SQLHENV henv, const WCHAR* connectionString);
	void			Clear();

	bool			Execute(const WCHAR* query);
	bool			Fetch();
	int32			GetRowCount();
	void			Unbind();

public:
	bool			BindParam(int32 paramIndex, bool* value, SQLLEN* index);
	bool			BindParam(int32 paramIndex, float* value, SQLLEN* index);
	bool			BindParam(int32 paramIndex, double* value, SQLLEN* index);
	bool			BindParam(int32 paramIndex, int8* value, SQLLEN* index);
	bool			BindParam(int32 paramIndex, int16* value, SQLLEN* index);
	bool			BindParam(int32 paramIndex, int32* value, SQLLEN* index);
	bool			BindParam(int32 paramIndex, int64* value, SQLLEN* index);
	bool			BindParam(int32 paramIndex, TIMESTAMP_STRUCT* value, SQLLEN* index);
	bool			BindParam(int32 paramIndex, const WCHAR* str, SQLLEN* index);
	bool			BindParam(int32 paramIndex, const BYTE* bin, int32 size, SQLLEN* index);

	bool			BindCol(int32 columIndex, bool* value, SQLLEN* index);
	bool			BindCol(int32 columIndex, float* value, SQLLEN* index);
	bool			BindCol(int32 columIndex, double* value, SQLLEN* index);
	bool			BindCol(int32 columIndex, int8* value, SQLLEN* index);
	bool			BindCol(int32 columIndex, int16* value, SQLLEN* index);
	bool			BindCol(int32 columIndex, int32* value, SQLLEN* index);
	bool			BindCol(int32 columIndex, int64* value, SQLLEN* index);
	bool			BindCol(int32 columIndex, TIMESTAMP_STRUCT* value, SQLLEN* index);
	bool			BindCol(int32 columIndex, WCHAR* str, int32 size, SQLLEN* index);
	bool			BindCol(int32 columIndex, BYTE* bin, int32 size, SQLLEN* index);

private:
	bool			BindParam(SQLUSMALLINT paramIndex, SQLSMALLINT cType, SQLSMALLINT sqlType, SQLULEN len, SQLPOINTER ptr, SQLLEN* index);
	bool			BindCol(SQLUSMALLINT columIndex, SQLSMALLINT cType, SQLULEN len, SQLPOINTER value, SQLLEN* index);
	void			HandleError(SQLRETURN ret);

private:
	SQLHDBC		_connection = SQL_NULL_HANDLE;
	SQLHSTMT	_statement = SQL_NULL_HANDLE;
};

