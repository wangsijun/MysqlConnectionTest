// ADOConn.h: interface for the ADOConn class.
#pragma once

class ADOConn
{
public:
	_ConnectionPtr m_pConnection;
	_RecordsetPtr m_pRecordset;
public:
	ADOConn();
	virtual ~ADOConn();
	// 初始化—连接数据库
	void OnInitADOConn();
	// 执行查询
	_RecordsetPtr& GetRecordSet(_bstr_t bstrSQL);
	// 执行SQL语句，Insert Update _variant_t
	BOOL ExecuteSQL(_bstr_t bstrSQL);
	//退出数据库
	void ExitConnect();
};