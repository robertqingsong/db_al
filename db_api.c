#include "db_api.h"

#include "mem_api.h"

#include ""

//open database.
CDatabase *db_open( const CDbParam *pDbParam )
{
	CDatabase *pRetCode = NULL;
	
	if ( pDbParam )
	{
		pRetCode = mem_malloc( sizeof( *pRetCode ) );
		if ( pRetCode )
		{
			memset( pRetCode, 0x00, sizeof(*pRetCode) );
			
			switch ( pDbParam->eDbType )
			{
			case DB_TYPE_SQLITE:
			{
				
			}break ;
			default:
			{
			
			}break ;	
			}
		}
	}
	
	return pRetCode;	
}

//close database.
void db_close( CDatabase *pDatabase )
{
	
}

//exec sql.
int32_t db_exec_sql( const CDatabase *pDatabase, const int8_t *pSql )
{
	int32_t iRetCode = -1;
	
	return iRetCode;
}

//exec query.
int32_t db_exec_query( const CDatabase *pDatabase, const int8_t *pSql )
{
	int32_t iRetCode = -1;
	
	return iRetCode;
}

//db next row.
int32_t db_next_row( const CDatabase *pDatabase )
{
	int32_t iRetCode = -1;
	
	return iRetCode;
}

//db get column data.
int32_t db_get_column_int( const CDatabase *pDatabase, int32_t iFieldIndex )
{
	int32_t iRetCode = -1;
	
	return iRetCode;
}

int32_t db_get_column_string( const CDatabase *pDatabase, int32_t iFieldIndex )
{
	int32_t iRetCode = -1;
	
	return iRetCode;
}
