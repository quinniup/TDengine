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

#ifndef _TD_COMMON_TOKEN_H_
#define _TD_COMMON_TOKEN_H_

#define TK_OR                               1
#define TK_AND                              2
#define TK_UNION                            3
#define TK_ALL                              4
#define TK_MINUS                            5
#define TK_EXCEPT                           6
#define TK_INTERSECT                        7
#define TK_NK_BITAND                        8
#define TK_NK_BITOR                         9
#define TK_NK_LSHIFT                       10
#define TK_NK_RSHIFT                       11
#define TK_NK_PLUS                         12
#define TK_NK_MINUS                        13
#define TK_NK_STAR                         14
#define TK_NK_SLASH                        15
#define TK_NK_REM                          16
#define TK_NK_CONCAT                       17
#define TK_CREATE                          18
#define TK_ACCOUNT                         19
#define TK_NK_ID                           20
#define TK_PASS                            21
#define TK_NK_STRING                       22
#define TK_ALTER                           23
#define TK_PPS                             24
#define TK_TSERIES                         25
#define TK_STORAGE                         26
#define TK_STREAMS                         27
#define TK_QTIME                           28
#define TK_DBS                             29
#define TK_USERS                           30
#define TK_CONNS                           31
#define TK_STATE                           32
#define TK_USER                            33
#define TK_ENABLE                          34
#define TK_NK_INTEGER                      35
#define TK_SYSINFO                         36
#define TK_DROP                            37
#define TK_GRANT                           38
#define TK_ON                              39
#define TK_TO                              40
#define TK_REVOKE                          41
#define TK_FROM                            42
#define TK_SUBSCRIBE                       43
#define TK_NK_COMMA                        44
#define TK_READ                            45
#define TK_WRITE                           46
#define TK_NK_DOT                          47
#define TK_WITH                            48
#define TK_DNODE                           49
#define TK_PORT                            50
#define TK_DNODES                          51
#define TK_RESTORE                         52
#define TK_NK_IPTOKEN                      53
#define TK_FORCE                           54
#define TK_UNSAFE                          55
#define TK_LOCAL                           56
#define TK_QNODE                           57
#define TK_BNODE                           58
#define TK_SNODE                           59
#define TK_MNODE                           60
#define TK_VNODE                           61
#define TK_DATABASE                        62
#define TK_USE                             63
#define TK_FLUSH                           64
#define TK_TRIM                            65
#define TK_COMPACT                         66
#define TK_IF                              67
#define TK_NOT                             68
#define TK_EXISTS                          69
#define TK_BUFFER                          70
#define TK_CACHEMODEL                      71
#define TK_CACHESIZE                       72
#define TK_COMP                            73
#define TK_DURATION                        74
#define TK_NK_VARIABLE                     75
#define TK_MAXROWS                         76
#define TK_MINROWS                         77
#define TK_KEEP                            78
#define TK_PAGES                           79
#define TK_PAGESIZE                        80
#define TK_TSDB_PAGESIZE                   81
#define TK_PRECISION                       82
#define TK_REPLICA                         83
#define TK_VGROUPS                         84
#define TK_SINGLE_STABLE                   85
#define TK_RETENTIONS                      86
#define TK_SCHEMALESS                      87
#define TK_WAL_LEVEL                       88
#define TK_WAL_FSYNC_PERIOD                89
#define TK_WAL_RETENTION_PERIOD            90
#define TK_WAL_RETENTION_SIZE              91
#define TK_WAL_ROLL_PERIOD                 92
#define TK_WAL_SEGMENT_SIZE                93
#define TK_STT_TRIGGER                     94
#define TK_TABLE_PREFIX                    95
#define TK_TABLE_SUFFIX                    96
#define TK_NK_COLON                        97
#define TK_BWLIMIT                         98
#define TK_START                           99
#define TK_TIMESTAMP                      100
#define TK_END                            101
#define TK_TABLE                          102
#define TK_NK_LP                          103
#define TK_NK_RP                          104
#define TK_STABLE                         105
#define TK_ADD                            106
#define TK_COLUMN                         107
#define TK_MODIFY                         108
#define TK_RENAME                         109
#define TK_TAG                            110
#define TK_SET                            111
#define TK_NK_EQ                          112
#define TK_USING                          113
#define TK_TAGS                           114
#define TK_BOOL                           115
#define TK_TINYINT                        116
#define TK_SMALLINT                       117
#define TK_INT                            118
#define TK_INTEGER                        119
#define TK_BIGINT                         120
#define TK_FLOAT                          121
#define TK_DOUBLE                         122
#define TK_BINARY                         123
#define TK_NCHAR                          124
#define TK_UNSIGNED                       125
#define TK_JSON                           126
#define TK_VARCHAR                        127
#define TK_MEDIUMBLOB                     128
#define TK_BLOB                           129
#define TK_VARBINARY                      130
#define TK_GEOMETRY                       131
#define TK_DECIMAL                        132
#define TK_COMMENT                        133
#define TK_MAX_DELAY                      134
#define TK_WATERMARK                      135
#define TK_ROLLUP                         136
#define TK_TTL                            137
#define TK_SMA                            138
#define TK_DELETE_MARK                    139
#define TK_FIRST                          140
#define TK_LAST                           141
#define TK_SHOW                           142
#define TK_PRIVILEGES                     143
#define TK_DATABASES                      144
#define TK_TABLES                         145
#define TK_STABLES                        146
#define TK_MNODES                         147
#define TK_QNODES                         148
#define TK_FUNCTIONS                      149
#define TK_INDEXES                        150
#define TK_ACCOUNTS                       151
#define TK_APPS                           152
#define TK_CONNECTIONS                    153
#define TK_LICENCES                       154
#define TK_GRANTS                         155
#define TK_QUERIES                        156
#define TK_SCORES                         157
#define TK_TOPICS                         158
#define TK_VARIABLES                      159
#define TK_CLUSTER                        160
#define TK_BNODES                         161
#define TK_SNODES                         162
#define TK_TRANSACTIONS                   163
#define TK_DISTRIBUTED                    164
#define TK_CONSUMERS                      165
#define TK_SUBSCRIPTIONS                  166
#define TK_VNODES                         167
#define TK_ALIVE                          168
#define TK_LIKE                           169
#define TK_TBNAME                         170
#define TK_QTAGS                          171
#define TK_AS                             172
#define TK_INDEX                          173
#define TK_FUNCTION                       174
#define TK_INTERVAL                       175
#define TK_COUNT                          176
#define TK_LAST_ROW                       177
#define TK_META                           178
#define TK_ONLY                           179
#define TK_TOPIC                          180
#define TK_CONSUMER                       181
#define TK_GROUP                          182
#define TK_DESC                           183
#define TK_DESCRIBE                       184
#define TK_RESET                          185
#define TK_QUERY                          186
#define TK_CACHE                          187
#define TK_EXPLAIN                        188
#define TK_ANALYZE                        189
#define TK_VERBOSE                        190
#define TK_NK_BOOL                        191
#define TK_RATIO                          192
#define TK_NK_FLOAT                       193
#define TK_OUTPUTTYPE                     194
#define TK_AGGREGATE                      195
#define TK_BUFSIZE                        196
#define TK_LANGUAGE                       197
#define TK_REPLACE                        198
#define TK_STREAM                         199
#define TK_INTO                           200
#define TK_PAUSE                          201
#define TK_RESUME                         202
#define TK_TRIGGER                        203
#define TK_AT_ONCE                        204
#define TK_WINDOW_CLOSE                   205
#define TK_IGNORE                         206
#define TK_EXPIRED                        207
#define TK_FILL_HISTORY                   208
#define TK_UPDATE                         209
#define TK_SUBTABLE                       210
#define TK_UNTREATED                      211
#define TK_KILL                           212
#define TK_CONNECTION                     213
#define TK_TRANSACTION                    214
#define TK_BALANCE                        215
#define TK_VGROUP                         216
#define TK_LEADER                         217
#define TK_MERGE                          218
#define TK_REDISTRIBUTE                   219
#define TK_SPLIT                          220
#define TK_DELETE                         221
#define TK_INSERT                         222
#define TK_NULL                           223
#define TK_NK_QUESTION                    224
#define TK_NK_ARROW                       225
#define TK_ROWTS                          226
#define TK_QSTART                         227
#define TK_QEND                           228
#define TK_QDURATION                      229
#define TK_WSTART                         230
#define TK_WEND                           231
#define TK_WDURATION                      232
#define TK_IROWTS                         233
#define TK_ISFILLED                       234
#define TK_CAST                           235
#define TK_NOW                            236
#define TK_TODAY                          237
#define TK_TIMEZONE                       238
#define TK_CLIENT_VERSION                 239
#define TK_SERVER_VERSION                 240
#define TK_SERVER_STATUS                  241
#define TK_CURRENT_USER                   242
#define TK_CASE                           243
#define TK_WHEN                           244
#define TK_THEN                           245
#define TK_ELSE                           246
#define TK_BETWEEN                        247
#define TK_IS                             248
#define TK_NK_LT                          249
#define TK_NK_GT                          250
#define TK_NK_LE                          251
#define TK_NK_GE                          252
#define TK_NK_NE                          253
#define TK_MATCH                          254
#define TK_NMATCH                         255
#define TK_CONTAINS                       256
#define TK_IN                             257
#define TK_JOIN                           258
#define TK_INNER                          259
#define TK_SELECT                         260
#define TK_NK_HINT                        261
#define TK_DISTINCT                       262
#define TK_WHERE                          263
#define TK_PARTITION                      264
#define TK_BY                             265
#define TK_SESSION                        266
#define TK_STATE_WINDOW                   267
#define TK_EVENT_WINDOW                   268
#define TK_SLIDING                        269
#define TK_FILL                           270
#define TK_VALUE                          271
#define TK_VALUE_F                        272
#define TK_NONE                           273
#define TK_PREV                           274
#define TK_NULL_F                         275
#define TK_LINEAR                         276
#define TK_NEXT                           277
#define TK_HAVING                         278
#define TK_RANGE                          279
#define TK_EVERY                          280
#define TK_ORDER                          281
#define TK_SLIMIT                         282
#define TK_SOFFSET                        283
#define TK_LIMIT                          284
#define TK_OFFSET                         285
#define TK_ASC                            286
#define TK_NULLS                          287
#define TK_ABORT                          288
#define TK_AFTER                          289
#define TK_ATTACH                         290
#define TK_BEFORE                         291
#define TK_BEGIN                          292
#define TK_BITAND                         293
#define TK_BITNOT                         294
#define TK_BITOR                          295
#define TK_BLOCKS                         296
#define TK_CHANGE                         297
#define TK_COMMA                          298
#define TK_CONCAT                         299
#define TK_CONFLICT                       300
#define TK_COPY                           301
#define TK_DEFERRED                       302
#define TK_DELIMITERS                     303
#define TK_DETACH                         304
#define TK_DIVIDE                         305
#define TK_DOT                            306
#define TK_EACH                           307
#define TK_FAIL                           308
#define TK_FILE                           309
#define TK_FOR                            310
#define TK_GLOB                           311
#define TK_ID                             312
#define TK_IMMEDIATE                      313
#define TK_IMPORT                         314
#define TK_INITIALLY                      315
#define TK_INSTEAD                        316
#define TK_ISNULL                         317
#define TK_KEY                            318
#define TK_MODULES                        319
#define TK_NK_BITNOT                      320
#define TK_NK_SEMI                        321
#define TK_NOTNULL                        322
#define TK_OF                             323
#define TK_PLUS                           324
#define TK_PRIVILEGE                      325
#define TK_RAISE                          326
#define TK_RESTRICT                       327
#define TK_ROW                            328
#define TK_SEMI                           329
#define TK_STAR                           330
#define TK_STATEMENT                      331
#define TK_STRICT                         332
#define TK_STRING                         333
#define TK_TIMES                          334
#define TK_VALUES                         335
#define TK_VARIABLE                       336
#define TK_VIEW                           337
#define TK_WAL                            338



#define TK_NK_SPACE         600
#define TK_NK_COMMENT       601
#define TK_NK_ILLEGAL       602
#define TK_NK_HEX           603  // hex number  0x123
#define TK_NK_OCT           604  // oct number
#define TK_NK_BIN           605  // bin format data 0b111
#define TK_BATCH_SCAN       606
#define TK_NO_BATCH_SCAN    607


#define TK_NK_NIL 65535

#endif /*_TD_COMMON_TOKEN_H_*/
