#include "db_api.h"

#include "mem_api.h"

#include "sqlite3.h"

#include "log.h"

typedef struct CSqlite_t
{
	sqlite3 *db;
   sqlite3_stmt *stmt;
}CSqlite;

//open database.
CDatabase *db_open( const CDbParam *pDbParam )
{
	CDatabase *pRetCode = NULL, *pNewDatabase = NULL;
	
	if ( pDbParam )
	{
		pNewDatabase = mem_malloc( sizeof( *pNewDatabase ) );
		if ( pNewDatabase )
		{
			memset( pNewDatabase, 0x00, sizeof(*pNewDatabase) );
			
			switch ( pDbParam->eDbType )
			{
			case DB_TYPE_SQLITE:
			{
				CSqlite *pSqlite = NULL;
				
				pSqlite = mem_malloc( sizeof( *pSqlite ) );
				if ( pSqlite )
				{
					memset( pSqlite, 0x00, sizeof(*pSqlite) );
					
					if ( init_mutex( &( pNewDatabase->Locker ) ) >= 0 )
					{
						lock( &( pNewDatabase->Locker ) );
						
						log_print( pDbParam->pDbName );
						
						if ( SQLITE_OK == sqlite3_open( (pDbParam->pDbName), &(pSqlite->db) ) )
						{
							pNewDatabase->eDbType = pDbParam->eDbType;
							pNewDatabase->pDbHandle = pSqlite;
						
							pRetCode = pNewDatabase;
						}
						else 
							log_print( "open sqlite db failed???????????????????" );
							
						unlock( &( pNewDatabase->Locker ) );
					}
					else 
						log_print( "init db mutex failed????????????????" );
				}
				else 
					log_print( "malloc memory for sqlite failed???????????" );
			}break ;
			default:
			{
			
			}break ;	
			}
			
			if ( NULL == pRetCode )
			{
				mem_free( pNewDatabase );
				pNewDatabase = NULL;
			}
		}
		else 
			log_print( "malloc memory for db failed??????????????????" );
	}
	else 
		log_print( "db param is NULL." );
	
	return pRetCode;	
}

//close database.
void db_close( CDatabase *pDatabase )
{
	if ( pDatabase )
	{
		CSqlite *pSqlite = NULL;
		
		lock( &( pDatabase->Locker ) );
		
		pSqlite = pDatabase->pDbHandle;
		if ( pSqlite )
		{
			if ( pSqlite->stmt )
			{
				sqlite3_finalize( pSqlite->stmt );
				pSqlite->stmt = NULL;	
			}
			
			sqlite3_close( pSqlite->db );
			pSqlite->db = NULL;
		}
		
		unlock( &( pDatabase->Locker ) );
	}
}

//exec sql.
int32_t db_exec_sql( const CDatabase *pDatabase, const int8_t *pSql )
{
	int32_t iRetCode = -1;
	
	if ( pDatabase && pSql )
	{
		lock( &( pDatabase->Locker ) );
		
		switch ( pDatabase->eDbType )
		{
		case DB_TYPE_SQLITE:
		{
			CSqlite *pSqlite = (CSqlite *)pDatabase->pDbHandle;
			
			if ( pSqlite )
			{
				int32_t iOpRet = sqlite3_exec( pSqlite->db, 
														 pSql, NULL, 0, &(pDatabase->zErrMsg) );
				if ( iOpRet == SQLITE_OK )
					iRetCode = 0;
			}
		}break;
		default:
		{
			
		}break ;
		}
		
		unlock( &( pDatabase->Locker ) );
	}
	
	return iRetCode;
}

//exec query.
int32_t db_exec_query( const CDatabase *pDatabase, const int8_t *pSql )
{
	int32_t iRetCode = -1;
	
	if ( pDatabase && pSql )
	{
		lock( &( pDatabase->Locker ) );
		
		switch ( pDatabase->eDbType )
		{
		case DB_TYPE_SQLITE:
		{
			CSqlite *pSqlite = (CSqlite *)pDatabase->pDbHandle;
			
			if ( pSqlite )
			{
				int32_t iOpRet = -1;
				
				if ( pSqlite->stmt )
				{
					sqlite3_finalize( pSqlite->stmt );
					pSqlite->stmt = NULL;	
				}
				
				iOpRet = sqlite3_prepare_v2( pSqlite->db, pSql, -1, &(pSqlite->stmt), 0 );
				
				if ( iOpRet == SQLITE_OK )
					iRetCode = 0;
			}
		}break;
		default:
		{
			
		}break ;
		}
		
		unlock( &( pDatabase->Locker ) );
	}
	
	return iRetCode;
}

//db next row.
int32_t db_next_row( const CDatabase *pDatabase )
{
	int32_t iRetCode = -1;
	
	if ( pDatabase )
	{
		lock( &( pDatabase->Locker ) );
		
		switch ( pDatabase->eDbType )
		{
		case DB_TYPE_SQLITE:
		{
			CSqlite *pSqlite = (CSqlite *)pDatabase->pDbHandle;
			
			if ( pSqlite )
			{	
				if ( pSqlite->stmt )
				{
					if ( SQLITE_ROW == sqlite3_step(pSqlite->stmt) )
						iRetCode = 0;
					else 
					{
						sqlite3_finalize( pSqlite->stmt );
						pSqlite->stmt = NULL;	
					}
				}
			}
		}break;
		default:
		{
			
		}break ;
		}
		
		unlock( &( pDatabase->Locker ) );
	}
	
	return iRetCode;
}

//db get column data.
int32_t db_get_column_int( const CDatabase *pDatabase, int32_t iFieldIndex )
{
	int32_t iRetCode = -1;
	
	if ( pDatabase && iFieldIndex >= 0 )
	{
		lock( &( pDatabase->Locker ) );
		
		switch ( pDatabase->eDbType )
		{
		case DB_TYPE_SQLITE:
		{
			CSqlite *pSqlite = (CSqlite *)pDatabase->pDbHandle;
			
			if ( pSqlite )
			{	
				if ( pSqlite->stmt )
				{
					iRetCode = sqlite3_column_int( pSqlite->stmt, iFieldIndex );
				}
			}
		}break;
		default:
		{
			
		}break ;
		}
		
		unlock( &( pDatabase->Locker ) );
	}
	
	return iRetCode;
}

int32_t db_get_column_string( const CDatabase *pDatabase, int32_t iFieldIndex, int8_t *pDataBuf, const int32_t iDataBufLen )
{
	int32_t iRetCode = -1;
	
	if ( pDatabase && iFieldIndex >= 0 && pDataBuf && iDataBufLen > 0 )
	{
		lock( &( pDatabase->Locker ) );
		
		switch ( pDatabase->eDbType )
		{
		case DB_TYPE_SQLITE:
		{
			CSqlite *pSqlite = (CSqlite *)pDatabase->pDbHandle;
			
			if ( pSqlite )
			{	
				if ( pSqlite->stmt )
				{
					const int8_t *pData = (const int8_t *)sqlite3_column_text( pSqlite->stmt, iFieldIndex );
					if ( pData )
					{
						int32_t iDataLen = strlen(pData);
						
						if ( iDataLen < iDataBufLen )
						{
							memcpy( pDataBuf, pData, iDataLen + 1 );
							
							iRetCode = 0;	
						}
					}
				}
			}
		}break;
		default:
		{
			
		}break ;
		}
		
		unlock( &( pDatabase->Locker ) );
	}
	
	return iRetCode;
}
