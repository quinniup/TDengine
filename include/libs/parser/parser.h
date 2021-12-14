/*
 * Copyright (c) 2019 TAOS Data, Inc. <jhtao@taosdata.com>
 *
 * This program is free software: you can use, redistribute, and/or modify
 * it under the terms of the GNU Affero General Public License, version 3
 * or later ("AGPL"), as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _TD_PARSER_H_
#define _TD_PARSER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "catalog.h"
#include "common.h"
#include "tname.h"
#include "tvariant.h"
#include "function.h"

typedef struct SField {
  char     name[TSDB_COL_NAME_LEN];
  uint8_t  type;
  int16_t  bytes;
} SField;

typedef struct SFieldInfo {
  int16_t     numOfOutput;   // number of column in result
  SField     *final;
  SArray     *internalField; // SArray<SInternalField>
} SFieldInfo;

typedef struct SCond {
  uint64_t uid;
  int32_t  len;  // length of tag query condition data
  char *   cond;
} SCond;

typedef struct SJoinNode {
  uint64_t uid;
  int16_t  tagColId;
  SArray*  tsJoin;
  SArray*  tagJoin;
} SJoinNode;

typedef struct SJoinInfo {
  bool       hasJoin;
  SJoinNode *joinTables[TSDB_MAX_JOIN_TABLE_NUM];
} SJoinInfo;

typedef struct STagCond {
  int16_t   relType;     // relation between tbname list and query condition, including : TK_AND or TK_OR
  SCond     tbnameCond;  // tbname query condition, only support tbname query condition on one table
  SJoinInfo joinInfo;    // join condition, only support two tables join currently
  SArray   *pCond;       // for different table, the query condition must be seperated
} STagCond;

typedef struct STableMetaInfo {
  STableMeta    *pTableMeta;      // table meta, cached in client side and acquired by name
  SVgroupsInfo  *vgroupList;
  SName         name;
  char          aliasName[TSDB_TABLE_NAME_LEN];    // alias name of table specified in query sql
  SArray       *tagColList;                        // SArray<SColumn*>, involved tag columns
} STableMetaInfo;

typedef struct SQueryStmtInfo {
  int16_t          command;       // the command may be different for each subclause, so keep it seperately.
  uint32_t         type;          // query/insert type
  STimeWindow      window;        // the whole query time window
  SInterval        interval;      // tumble time window
  SSessionWindow   sessionWindow; // session time window
  SStateWindow     stateWindow;   // state window query
  SGroupbyExpr     groupbyExpr;   // groupby tags info
  SArray *         colList;       // SArray<SColumn*>
  SFieldInfo       fieldsInfo;
  SArray**         exprList;      // SArray<SExprInfo*>
  SLimit           limit;
  SLimit           slimit;
  STagCond         tagCond;
  SArray *         colCond;
  SArray *         order;
  int16_t          numOfTables;
  int16_t          curTableIdx;
  STableMetaInfo **pTableMetaInfo;
  struct STSBuf   *tsBuf;

  int16_t          fillType;      // final result fill type
  int64_t *        fillVal;       // default value for fill
  int32_t          numOfFillVal;  // fill value size

  char *           msg;           // pointer to the pCmd->payload to keep error message temporarily
  int64_t          clauseLimit;   // limit for current sub clause

  int64_t          prjOffset;     // offset value in the original sql expression, only applied at client side
  int64_t          vgroupLimit;    // table limit in case of super table projection query + global order + limit

  int32_t          udColumnId;    // current user-defined constant output field column id, monotonically decreases from TSDB_UD_COLUMN_INDEX
  int32_t          bufLen;
  char*            buf;
  SArray          *pUdfInfo;

  struct SQueryStmtInfo *sibling;     // sibling
  struct SQueryStmtInfo *pDownstream;
  SMultiFunctionsDesc    info;
  SArray            *pUpstream;   // SArray<struct SQueryStmtInfo>
  int32_t            havingFieldNum;
  int32_t            exprListLevelIndex;
} SQueryStmtInfo;

typedef struct SColumnIndex {
  int16_t tableIndex;
  int16_t columnIndex;
  int16_t type;               // normal column/tag/ user input constant column
} SColumnIndex;

struct SInsertStmtInfo;

/**
 * True will be returned if the input sql string is insert, false otherwise.
 * @param pStr    sql string
 * @param length  length of the sql string
 * @return
 */
bool qIsInsertSql(const char* pStr, size_t length);

typedef struct SParseContext {
  const char* pSql;    // sql string
  size_t sqlLen;       // length of the sql string
  int64_t id;          // operator id, generated by uuid generator
  const char* pDbname;
  const SEpSet* pEpSet; 
  int8_t schemaAttached; // denote if submit block is built with table schema or not

  char* pMsg;           // extended error message if exists to help avoid the problem in sql statement.
  int32_t msgLen;      // max length of the msg
} SParseContext;

/**
 * Parse the sql statement and then return the SQueryStmtInfo as the result of bounded AST.
 * @param pSql     sql string
 * @param length   length of the sql string
 * @param id       operator id, generated by uuid generator
 * @param msg      extended error message if exists.
 * @return         error code
 */
struct SQueryStmtInfo* qParseQuerySql(const char* pStr, size_t length, int64_t id, char* msg, int32_t msgLen);

typedef enum {
  PAYLOAD_TYPE_KV = 0,
  PAYLOAD_TYPE_RAW = 1,
} EPayloadType;

typedef struct SVgDataBlocks {
  int64_t     vgId;         // virtual group id
  int32_t     numOfTables;  // number of tables in current submit block
  uint32_t    size;
  char       *pData;        // SMsgDesc + SSubmitMsg + SSubmitBlk + ...
} SVgDataBlocks;

typedef struct SInsertStmtInfo {
  SArray*     pDataBlocks;         // data block for each vgroup, SArray<SVgDataBlocks*>.
  int8_t      schemaAttache;        // denote if submit block is built with table schema or not
  uint8_t     payloadType;         // EPayloadType. 0: K-V payload for non-prepare insert, 1: rawPayload for prepare insert
  uint32_t    insertType;          // insert data from [file|sql statement| bound statement]
  const char* sql;                 // current sql statement position
} SInsertStmtInfo;

/**
 * Parse the insert sql statement.
 * @param pStr            sql string
 * @param length          length of the sql string
 * @param id              operator id, generated by uuid generator.
 * @param msg             extended error message if exists to help avoid the problem in sql statement.
 * @return                data in binary format to submit to vnode directly.
 */
 int32_t qParseInsertSql(SParseContext* pContext, struct SInsertStmtInfo** pInfo);

/**
 * Convert a normal sql statement to only query tags information to enable that the subscribe client can be aware quickly of the true vgroup ids that
 * involved in the subscribe procedure.
 * @param pSql
 * @param length
 * @param pConvertSql
 * @return
 */
int32_t qParserConvertSql(const char* pStr, size_t length, char** pConvertSql);

void assignExprInfo(SExprInfo* dst, const SExprInfo* src);
void columnListCopy(SArray* dst, const SArray* src, uint64_t uid);
void columnListDestroy(SArray* pColumnList);

void dropAllExprInfo(SArray** pExprInfo, int32_t numOfLevel);

typedef struct SSourceParam {
  SArray            *pExprNodeList; //Array<struct tExprNode*>
  SArray            *pColumnList;   //Array<struct SColumn>
  int32_t    num;
} SSourceParam;

SExprInfo* createExprInfo(STableMetaInfo* pTableMetaInfo, const char* funcName, SSourceParam* pSource, SSchema* pResSchema, int16_t interSize);
int32_t copyExprInfoList(SArray* dst, const SArray* src, uint64_t uid, bool deepcopy);
int32_t getExprFunctionLevel(SQueryStmtInfo* pQueryInfo);

STableMetaInfo* getMetaInfo(SQueryStmtInfo* pQueryInfo, int32_t tableIndex);
SSchema *getOneColumnSchema(const STableMeta* pTableMeta, int32_t colIndex);
SSchema createSchema(uint8_t type, int16_t bytes, int16_t colId, const char* name);

int32_t getNewResColId();
void addIntoSourceParam(SSourceParam* pSourceParam, tExprNode* pNode, SColumn* pColumn);

#ifdef __cplusplus
}
#endif

#endif /*_TD_PARSER_H_*/