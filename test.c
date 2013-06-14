#include "db_api.h"

#include "log.h"

int main( int argc, char **argv )
{
	CDbParam stDbParam;
	CDatabase *pDatabase = NULL;

	enable_log( 1 );
	set_log( LOG_TYPE_CONSOLE, NULL, 0 );
	
	memset( &stDbParam, 0x00, sizeof(stDbParam) );
	memcpy( stDbParam.pDbName, "test.db", strlen("test.db") + 1 );
	stDbParam.eDbType = DB_TYPE_SQLITE;

	pDatabase = db_open( &stDbParam );
	if ( pDatabase )
	{
		const int8_t *pSql = NULL;
		
		pSql = "CREATE TABLE IF NOT EXISTS test (id INTEGER NOT NULL PRIMARY KEY, text VARCHAR(100))";
		if ( db_exec_sql(pDatabase, pSql) >= 0 )
		{
			pSql = "INSERT INTO test(text) VALUES (\"text1\")";
			if( db_exec_sql(pDatabase, pSql) >= 0 )
			{
				pSql = "SELECT * FROM test";
				if ( db_exec_query( pDatabase, pSql ) >= 0 )
				{
					int8_t pTempBuf[128] = { 0x00, };
					
					while ( db_next_row( pDatabase ) >= 0 )
					{
						if ( db_get_column_string( pDatabase, 1, pTempBuf, sizeof(pTempBuf) ) >= 0 )
						{
							log_print( "data-->%s", pTempBuf );
						}
						else 
							log_print( "get column data failed??????????????????" );
					}
				}
				else 
					log_print( "query db failed???????????????????" );
			}
			else 
				log_print( "insert db failed??????????????????" );
		}
		else 
			log_print( "create db failed??????????????????????" );
		
		db_close( pDatabase );
		pDatabase = NULL;
	}
	else 
		log_print( "open db failed?????????" );

	return 0;	
}
