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

#include "deploy.h"

class DndTestDnode : public ::testing::Test {
 public:
  static SServer* CreateServer(const char* path, const char* fqdn, uint16_t port, const char* firstEp) {
    SServer* pServer = createServer(path, fqdn, port, firstEp);
    ASSERT(pServer);
    return pServer;
  }

  static void SetUpTestSuite() {
    initLog("/tmp/dndTestDnode");

    const char* fqdn = "localhost";
    const char* firstEp = "localhost:9521";
    pServer1 = CreateServer("/tmp/dndTestDnode1", fqdn, 9521, firstEp);
    pServer2 = CreateServer("/tmp/dndTestDnode2", fqdn, 9522, firstEp);
    pServer3 = CreateServer("/tmp/dndTestDnode3", fqdn, 9523, firstEp);
    pServer4 = CreateServer("/tmp/dndTestDnode4", fqdn, 9524, firstEp);
    pServer5 = CreateServer("/tmp/dndTestDnode5", fqdn, 9525, firstEp);
    pClient = createClient("root", "taosdata", fqdn, 9521);
    taosMsleep(300);
  }

  static void TearDownTestSuite() {
    dropServer(pServer1);
    dropServer(pServer2);
    dropServer(pServer3);
    dropServer(pServer4);
    dropServer(pServer5);
    dropClient(pClient);
  }

  static SServer* pServer1;
  static SServer* pServer2;
  static SServer* pServer3;
  static SServer* pServer4;
  static SServer* pServer5;
  static SClient* pClient;

 public:
  void SetUp() override {}
  void TearDown() override {}

  void SendTheCheckShowMetaMsg(int8_t showType, const char* showName, int32_t columns) {
    SShowMsg* pShow = (SShowMsg*)rpcMallocCont(sizeof(SShowMsg));
    pShow->type = showType;
    strcpy(pShow->db, "");

    SRpcMsg showRpcMsg = {0};
    showRpcMsg.pCont = pShow;
    showRpcMsg.contLen = sizeof(SShowMsg);
    showRpcMsg.msgType = TSDB_MSG_TYPE_SHOW;

    sendMsg(pClient, &showRpcMsg);
    ASSERT_NE(pClient->pRsp, nullptr);
    ASSERT_EQ(pClient->pRsp->code, 0);
    ASSERT_NE(pClient->pRsp->pCont, nullptr);

    SShowRsp* pShowRsp = (SShowRsp*)pClient->pRsp->pCont;
    ASSERT_NE(pShowRsp, nullptr);
    pShowRsp->showId = htonl(pShowRsp->showId);
    pMeta = &pShowRsp->tableMeta;
    pMeta->numOfTags = htons(pMeta->numOfTags);
    pMeta->numOfColumns = htons(pMeta->numOfColumns);
    pMeta->sversion = htons(pMeta->sversion);
    pMeta->tversion = htons(pMeta->tversion);
    pMeta->tuid = htobe64(pMeta->tuid);
    pMeta->suid = htobe64(pMeta->suid);

    showId = pShowRsp->showId;

    EXPECT_NE(pShowRsp->showId, 0);
    EXPECT_STREQ(pMeta->tbFname, showName);
    EXPECT_EQ(pMeta->numOfTags, 0);
    EXPECT_EQ(pMeta->numOfColumns, columns);
    EXPECT_EQ(pMeta->precision, 0);
    EXPECT_EQ(pMeta->tableType, 0);
    EXPECT_EQ(pMeta->update, 0);
    EXPECT_EQ(pMeta->sversion, 0);
    EXPECT_EQ(pMeta->tversion, 0);
    EXPECT_EQ(pMeta->tuid, 0);
    EXPECT_EQ(pMeta->suid, 0);
  }

  void CheckSchema(int32_t index, int8_t type, int32_t bytes, const char* name) {
    SSchema* pSchema = &pMeta->pSchema[index];
    pSchema->bytes = htons(pSchema->bytes);
    EXPECT_EQ(pSchema->colId, 0);
    EXPECT_EQ(pSchema->type, type);
    EXPECT_EQ(pSchema->bytes, bytes);
    EXPECT_STREQ(pSchema->name, name);
  }

  void SendThenCheckShowRetrieveMsg(int32_t rows, int32_t completed) {
    SRetrieveTableMsg* pRetrieve = (SRetrieveTableMsg*)rpcMallocCont(sizeof(SRetrieveTableMsg));
    pRetrieve->showId = htonl(showId);
    pRetrieve->free = 0;

    SRpcMsg retrieveRpcMsg = {0};
    retrieveRpcMsg.pCont = pRetrieve;
    retrieveRpcMsg.contLen = sizeof(SRetrieveTableMsg);
    retrieveRpcMsg.msgType = TSDB_MSG_TYPE_SHOW_RETRIEVE;

    sendMsg(pClient, &retrieveRpcMsg);

    ASSERT_NE(pClient->pRsp, nullptr);
    ASSERT_EQ(pClient->pRsp->code, 0);
    ASSERT_NE(pClient->pRsp->pCont, nullptr);

    pRetrieveRsp = (SRetrieveTableRsp*)pClient->pRsp->pCont;
    ASSERT_NE(pRetrieveRsp, nullptr);
    pRetrieveRsp->numOfRows = htonl(pRetrieveRsp->numOfRows);
    pRetrieveRsp->offset = htobe64(pRetrieveRsp->offset);
    pRetrieveRsp->useconds = htobe64(pRetrieveRsp->useconds);
    pRetrieveRsp->compLen = htonl(pRetrieveRsp->compLen);

    EXPECT_EQ(pRetrieveRsp->numOfRows, rows);
    EXPECT_EQ(pRetrieveRsp->offset, 0);
    EXPECT_EQ(pRetrieveRsp->useconds, 0);
    EXPECT_EQ(pRetrieveRsp->completed, completed);
    EXPECT_EQ(pRetrieveRsp->precision, TSDB_TIME_PRECISION_MILLI);
    EXPECT_EQ(pRetrieveRsp->compressed, 0);
    EXPECT_EQ(pRetrieveRsp->reserved, 0);
    EXPECT_EQ(pRetrieveRsp->compLen, 0);

    pData = pRetrieveRsp->data;
    pos = 0;
  }

  void CheckInt16(int16_t val) {
    int16_t data = *((int16_t*)(pData + pos));
    pos += sizeof(int16_t);
    EXPECT_EQ(data, val);
  }

  void CheckInt64(int64_t val) {
    int64_t data = *((int64_t*)(pData + pos));
    pos += sizeof(int64_t);
    EXPECT_EQ(data, val);
  }

  void CheckTimestamp() {
    int64_t data = *((int64_t*)(pData + pos));
    pos += sizeof(int64_t);
    EXPECT_GT(data, 0);
  }

  void CheckBinary(const char* val, int32_t len) {
    pos += sizeof(VarDataLenT);
    char* data = (char*)(pData + pos);
    pos += len;
    EXPECT_STREQ(data, val);
  }

  int32_t            showId;
  STableMetaMsg*     pMeta;
  SRetrieveTableRsp* pRetrieveRsp;
  char*              pData;
  int32_t            pos;
};

SServer* DndTestDnode::pServer1;
SServer* DndTestDnode::pServer2;
SServer* DndTestDnode::pServer3;
SServer* DndTestDnode::pServer4;
SServer* DndTestDnode::pServer5;
SClient* DndTestDnode::pClient;

TEST_F(DndTestDnode, ShowDnode) {
  SendTheCheckShowMetaMsg(TSDB_MGMT_TABLE_DNODE, "show dnodes", 7);
  CheckSchema(0, TSDB_DATA_TYPE_SMALLINT, 2, "id");
  CheckSchema(1, TSDB_DATA_TYPE_BINARY, TSDB_EP_LEN + VARSTR_HEADER_SIZE, "end point");
  CheckSchema(2, TSDB_DATA_TYPE_SMALLINT, 2, "vnodes");
  CheckSchema(3, TSDB_DATA_TYPE_SMALLINT, 2, "max vnodes");
  CheckSchema(4, TSDB_DATA_TYPE_BINARY, 10 + VARSTR_HEADER_SIZE, "status");
  CheckSchema(5, TSDB_DATA_TYPE_TIMESTAMP, 8, "create time");
  CheckSchema(6, TSDB_DATA_TYPE_BINARY, 24 + VARSTR_HEADER_SIZE, "offline reason");

  SendThenCheckShowRetrieveMsg(1, 1);
  CheckInt16(1);
  CheckBinary("localhost:9521", TSDB_EP_LEN);
  CheckInt16(0);
  CheckInt16(1);
  CheckBinary("ready", 10);
  CheckTimestamp();
  CheckBinary("", 24);
}

TEST_F(DndTestDnode, CreateDnode_01) {
  SCreateDnodeMsg* pReq = (SCreateDnodeMsg*)rpcMallocCont(sizeof(SCreateDnodeMsg));
  strcpy(pReq->ep, "localhost:9522");

  SRpcMsg rpcMsg = {0};
  rpcMsg.pCont = pReq;
  rpcMsg.contLen = sizeof(SCreateDnodeMsg);
  rpcMsg.msgType = TSDB_MSG_TYPE_CREATE_DNODE;

  sendMsg(pClient, &rpcMsg);
  SRpcMsg* pMsg = pClient->pRsp;
  ASSERT_NE(pMsg, nullptr);
  ASSERT_EQ(pMsg->code, 0);

  taosMsleep(1300);
  SendTheCheckShowMetaMsg(TSDB_MGMT_TABLE_DNODE, "show dnodes", 7);
  SendThenCheckShowRetrieveMsg(2, 1);
  CheckInt16(1);
  CheckInt16(2);
  CheckBinary("localhost:9521", TSDB_EP_LEN);
  CheckBinary("localhost:9522", TSDB_EP_LEN);
  CheckInt16(0);
  CheckInt16(0);
  CheckInt16(1);
  CheckInt16(1);
  CheckBinary("ready", 10);
  CheckBinary("ready", 10);
  CheckTimestamp();
  CheckTimestamp();
  CheckBinary("", 24);
  CheckBinary("", 24);
}

#if 0
TEST_F(DndTestDnode, AlterUser_01) {
  ASSERT_NE(pClient, nullptr);

  //--- drop user ---
  SAlterUserMsg* pReq = (SAlterUserMsg*)rpcMallocCont(sizeof(SAlterUserMsg));
  strcpy(pReq->user, "u1");
  strcpy(pReq->pass, "p2");

  SRpcMsg rpcMsg = {0};
  rpcMsg.pCont = pReq;
  rpcMsg.contLen = sizeof(SAlterUserMsg);
  rpcMsg.msgType = TSDB_MSG_TYPE_ALTER_USER;

  sendMsg(pClient, &rpcMsg);
  SRpcMsg* pMsg = pClient->pRsp;
  ASSERT_NE(pMsg, nullptr);
  ASSERT_EQ(pMsg->code, 0);

  //--- meta ---
  SShowMsg* pShow = (SShowMsg*)rpcMallocCont(sizeof(SShowMsg));
  pShow->type = TSDB_MGMT_TABLE_USER;
  SRpcMsg showRpcMsg = {0};
  showRpcMsg.pCont = pShow;
  showRpcMsg.contLen = sizeof(SShowMsg);
  showRpcMsg.msgType = TSDB_MSG_TYPE_SHOW;

  sendMsg(pClient, &showRpcMsg);
  SShowRsp*      pShowRsp = (SShowRsp*)pClient->pRsp->pCont;
  STableMetaMsg* pMeta = &pShowRsp->tableMeta;
  pMeta->numOfColumns = htons(pMeta->numOfColumns);
  EXPECT_EQ(pMeta->numOfColumns, 4);

  //--- retrieve ---
  SRetrieveTableMsg* pRetrieve = (SRetrieveTableMsg*)rpcMallocCont(sizeof(SRetrieveTableMsg));
  pRetrieve->showId = pShowRsp->showId;
  SRpcMsg retrieveRpcMsg = {0};
  retrieveRpcMsg.pCont = pRetrieve;
  retrieveRpcMsg.contLen = sizeof(SRetrieveTableMsg);
  retrieveRpcMsg.msgType = TSDB_MSG_TYPE_SHOW_RETRIEVE;

  sendMsg(pClient, &retrieveRpcMsg);
  SRetrieveTableRsp* pRetrieveRsp = (SRetrieveTableRsp*)pClient->pRsp->pCont;
  pRetrieveRsp->numOfRows = htonl(pRetrieveRsp->numOfRows);
  EXPECT_EQ(pRetrieveRsp->numOfRows, 3);

  char*   pData = pRetrieveRsp->data;
  int32_t pos = 0;
  char*   strVal = NULL;

  //--- name ---
  {
    pos += sizeof(VarDataLenT);
    strVal = (char*)(pData + pos);
    pos += TSDB_USER_LEN;
    EXPECT_STREQ(strVal, "u1");

    pos += sizeof(VarDataLenT);
    strVal = (char*)(pData + pos);
    pos += TSDB_USER_LEN;
    EXPECT_STREQ(strVal, "root");

    pos += sizeof(VarDataLenT);
    strVal = (char*)(pData + pos);
    pos += TSDB_USER_LEN;
    EXPECT_STREQ(strVal, "_root");
  }
}

#endif

TEST_F(DndTestDnode, DropDnode_01) {
  SDropDnodeMsg* pReq = (SDropDnodeMsg*)rpcMallocCont(sizeof(SDropDnodeMsg));
  pReq->dnodeId = htonl(2);

  SRpcMsg rpcMsg = {0};
  rpcMsg.pCont = pReq;
  rpcMsg.contLen = sizeof(SDropDnodeMsg);
  rpcMsg.msgType = TSDB_MSG_TYPE_DROP_DNODE;

  sendMsg(pClient, &rpcMsg);
  SRpcMsg* pMsg = pClient->pRsp;
  ASSERT_NE(pMsg, nullptr);
  ASSERT_EQ(pMsg->code, 0);

  taosMsleep(1300);
  SendTheCheckShowMetaMsg(TSDB_MGMT_TABLE_DNODE, "show dnodes", 7);
  SendThenCheckShowRetrieveMsg(1, 0);
  CheckInt16(1);
  CheckBinary("localhost:9521", TSDB_EP_LEN);
  CheckInt16(0);
  CheckInt16(1);
  CheckBinary("ready", 10);
  CheckTimestamp();
  CheckBinary("", 24);
}

TEST_F(DndTestDnode, CreateDnode_02) {
  {
    SCreateDnodeMsg* pReq = (SCreateDnodeMsg*)rpcMallocCont(sizeof(SCreateDnodeMsg));
    strcpy(pReq->ep, "localhost:9523");

    SRpcMsg rpcMsg = {0};
    rpcMsg.pCont = pReq;
    rpcMsg.contLen = sizeof(SCreateDnodeMsg);
    rpcMsg.msgType = TSDB_MSG_TYPE_CREATE_DNODE;

    sendMsg(pClient, &rpcMsg);
    SRpcMsg* pMsg = pClient->pRsp;
    ASSERT_NE(pMsg, nullptr);
    ASSERT_EQ(pMsg->code, 0);
  }

  {
    SCreateDnodeMsg* pReq = (SCreateDnodeMsg*)rpcMallocCont(sizeof(SCreateDnodeMsg));
    strcpy(pReq->ep, "localhost:9524");

    SRpcMsg rpcMsg = {0};
    rpcMsg.pCont = pReq;
    rpcMsg.contLen = sizeof(SCreateDnodeMsg);
    rpcMsg.msgType = TSDB_MSG_TYPE_CREATE_DNODE;

    sendMsg(pClient, &rpcMsg);
    SRpcMsg* pMsg = pClient->pRsp;
    ASSERT_NE(pMsg, nullptr);
    ASSERT_EQ(pMsg->code, 0);
  }

  {
    SCreateDnodeMsg* pReq = (SCreateDnodeMsg*)rpcMallocCont(sizeof(SCreateDnodeMsg));
    strcpy(pReq->ep, "localhost:9525");

    SRpcMsg rpcMsg = {0};
    rpcMsg.pCont = pReq;
    rpcMsg.contLen = sizeof(SCreateDnodeMsg);
    rpcMsg.msgType = TSDB_MSG_TYPE_CREATE_DNODE;

    sendMsg(pClient, &rpcMsg);
    SRpcMsg* pMsg = pClient->pRsp;
    ASSERT_NE(pMsg, nullptr);
    ASSERT_EQ(pMsg->code, 0);
  }

  taosMsleep(1300);
  SendTheCheckShowMetaMsg(TSDB_MGMT_TABLE_DNODE, "show dnodes", 7);
  SendThenCheckShowRetrieveMsg(4, 0);
  CheckInt16(1);
  CheckInt16(3);
  CheckInt16(4);
  CheckInt16(5);
  CheckBinary("localhost:9521", TSDB_EP_LEN);
  CheckBinary("localhost:9523", TSDB_EP_LEN);
  CheckBinary("localhost:9524", TSDB_EP_LEN);
  CheckBinary("localhost:9525", TSDB_EP_LEN);
  CheckInt16(0);
  CheckInt16(0);
  CheckInt16(0);
  CheckInt16(0);
  CheckInt16(1);
  CheckInt16(1);
  CheckInt16(1);
  CheckInt16(1);
  CheckBinary("ready", 10);
  CheckBinary("ready", 10);
  CheckBinary("ready", 10);
  CheckBinary("ready", 10);
  CheckTimestamp();
  CheckTimestamp();
  CheckTimestamp();
  CheckTimestamp();
  CheckBinary("", 24);
  CheckBinary("", 24);
  CheckBinary("", 24);
  CheckBinary("", 24);
}