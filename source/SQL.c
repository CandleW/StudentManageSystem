/**
 * @file SQL.h
 * @brief ���ݿ��ļ�
 * @details �������ݿ��������
 * @author ren
 * @date 2021-1-2
 * @version 1.2
 */


#include "../head/SQL.h"

#include <Windows.h>
#include <stdio.h>
#include <sqlext.h>
#include <string.h>
#include <stdarg.h>

char szDSN[256] = "Driver={Microsoft Access Driver (*.mdb, *.accdb)};DSN='';DBQ=C:\\Users\\wzt13\\OneDrive - email.ncu.edu.cn\\����\\Student(1).accdb;";

HENV hEnv;//�������
HDBC hDbc;//���Ӿ��
RETCODE rc;//
HSTMT hstmt;//�����

int SqlConn();//���ݿ����Ӻ���
void SqlDisConn();//���ݿ�Ͽ����Ӻ���
int SqlExecute(char *query);//SQL���ִ�к���
char *link(char *str, va_list list);//�ַ������Ӻ���
int TableConn(char *table);//�����Ӻ���

/**
 * @brief ������Access���ݿ�
 * @return �Ƿ����ӳɹ�
 * @retval 0 ʧ��
 * @retval 1 �ɹ�
 */
int SqlConn() {
    int iConnStrLength2Ptr;
    char szConnStrOut[256];
    rc = SQLAllocEnv(&hEnv);
    rc = SQLAllocConnect(hEnv, &hDbc);
    rc = SQLDriverConnect(hDbc, NULL, (unsigned char *) szDSN,
                          SQL_NTS, (unsigned char *) szConnStrOut,
                          255, (SQLSMALLINT *) &iConnStrLength2Ptr, SQL_DRIVER_NOPROMPT);
    if (SQL_SUCCEEDED(rc)) {
        //printf("Access Connect Succeeded!\n");
        return 1;
    } else {
        //printf("Access Connect Failed!\n");
        return 0;
    }
}

/**
 * @brief �Ͽ�Access���ݿ�����
 * @return �Ƿ�����ɹ�
 * @retval 0 ʧ��
 * @retval 1 �ɹ�
 */
void SqlDisConn() {
    SQLDisconnect(hDbc);
    SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
    SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
    //printf("Access DisConnect Succeed!\n");
}

/**
 * @brief ִ��SQL���
 * @param[in] table ����
 * @return �Ƿ�ɹ�
 * @retval 0    ʧ��
 * @retval 1    �ɹ�
 */
int SqlExecute(char *query) {
    rc = SQLAllocStmt(hDbc, &hstmt);
    rc = SQLPrepare(hstmt, (SQLCHAR *) query, SQL_NTS);
    rc = SQLExecute(hstmt);
    if (SQL_SUCCEEDED(rc)) {
        //printf("SQL Execute Succeeded!\n");
        return 1;
    } else {
        //printf("SQL Execute Failed!\n");
        return 0;
    }
}

/**
 * @brief �ַ�������
 * @param[in] str ԭ�ַ���
 * @param[in] list ԭ�ַ���
 * @return temp Ŀ���ַ���
 */
char *link(char *str, va_list list) {
    char *temp;//Ŀ���ַ���
    int len1;//ԭ�ַ���
    int len2;//ԭ�ַ���
    temp = (char *) malloc(128);
    memset(temp, 0, sizeof(temp));
    len1 = strlen(str) + 1;
    strcat_s(temp, len1, str);
    len2 = strlen(temp) + strlen(list) + 1;
    strcat_s(temp, len2, list);
    return temp;
}

/**
 * @brief ���������ݿ���ĳһ����
 * @return �Ƿ����ӳɹ�
 * @retval 0 ʧ��
 * @retval 1 �ɹ�
 */
int TableConn(char *table) {
    char *query = link("select * from ", table);
    if (SqlExecute(query)) {
        //printf("Table %s Connect Succeeded!\n",table);
        return 1;
    } else {
        //printf("Table %s Connect Failed!\n",table);
        return 0;
    }
}

/**
 * @brief �����ݿ�ָ�����������Ϣ
 * @param[in] table ����
 * @param[in] ... ��������ֵ����ĳ����Ϣ���������ö�Ӧ��Ĭ��ֵ���
 * @return �Ƿ�ɹ�
 * @retval 0    ʧ��
 * @retval 1    �ɹ�
 */
__attribute__((unused)) int SqlAdd(char *table, ...) {
    SQLSMALLINT colNum = 0;//����
    SQLSMALLINT colCount = 0;//��ǰ��
    va_list list;
    char *str2;
    char *str1;
    char *query;//SQL���
    if (SqlConn()) {
        va_start(list, table);
        str2 = link("(\'", va_arg(list, char*));
        if (TableConn(table)) {
            SQLNumResultCols(hstmt, &colNum);
            for (colCount = 1; colCount < colNum; colCount++) {
                str2 = link(str2, link("\'\,\'", va_arg(list, char*)));
            }
        }
        str2 = link(str2, "\')");
        va_end(list);
        str1 = link(link("insert into ", table), " values ");
        query = link(str1, str2);
        //printf("SQL:%s\n",query);
        if (SqlExecute(query)) {
            SqlDisConn();
            printf("Add Succeeded!\n");
            return 1;
        } else {
            SqlDisConn();
            printf("Add Failed!\n");
            return 0;
        }
    } else {
        printf("Couldn't connect to %s.\n", szDSN);
        return 0;
    }
}

/**
 * @brief ������ݿ���ָ����
 * @param[in] table ���ݿ���ָ���ı�
 * @return �Ƿ���ճɹ�
 * @retval 0 ʧ��
 * @retval 1 �ɹ�
 */
__attribute__((unused)) int SqlClear(char *table) {
    char *query;//SQL���
    query = link("delete * from ", table);
    if (SqlConn()) {
        if (TableConn(table)) {
            //printf("SQL:%s\n",query);
            if (SqlExecute(query)) {
                SqlDisConn();
                printf("Clear Succeeded!\n");
                return 1;
            } else {
                SqlDisConn();
                printf("Clear Failed!\n");
                return 0;
            }
        } else {
            printf("Clear Failed!\n");
            return 0;
        }
    } else {
        printf("Couldn't connect to %s.\n", szDSN);
        return 0;
    }
}


/**
 * @brief ɾ�����ݿ���ָ�����е�ĳһ��
 * @param[in] table ���ݿ���ָ���ı�
 * @param[in] keyName ������
 * @param[in] key ��������
 * @return �Ƿ�ɾ���ɹ�
 * @retval 0 ʧ��
 * @retval 1 �ɹ�
 */
__attribute__((unused)) int SqlDel(char *table, char *keyName, char *key) {
    char *query;//SQL���
    char *str1 = link(link("delete from ", table), " where ");
    char *str2 = link(link(str1, keyName), "=\'");
    query = link(link(str2, key), "\'");
    if (SqlConn()) {
        //printf("SQL:%s\n",query);
        if (SqlExecute(query)) {
            SqlDisConn();
            printf("Delete Succeeded!\n");
            return 1;
        } else {
            SqlDisConn();
            printf("Delete Failed!\n");
            return 0;
        }
    } else {
        printf("Couldn't connect to %s.\n", szDSN);
        return 0;
    }
}

/**
 * @brief �޸�ָ������ĳһ����ȫ��ֵ
 * @param[in] table ����
 * @param[in] keyName ������
 * @param[in] key ����
 * @param ... �޸ĺ��ֵ
 * @return �Ƿ��޸ĳɹ�
 * @retval 0 ʧ��
 * @retval 1 �ɹ�
 */
__attribute__((unused)) int SqlChangeRow(char *table, char *keyName, char *key, ...) {
    SQLSMALLINT colNum = 0;//����
    SQLSMALLINT colCount = 0;//��ǰ��
    SQLCHAR colName[128] = {'\0'};//����
    int n = 1;//����ֵ
    char name[20][128];//�����������
    va_list list;
    if (SqlConn()) {
        va_start(list, key);
        if (TableConn(table)) {
            SQLNumResultCols(hstmt, &colNum);
            for (colCount = 1; colCount <= colNum; colCount++) {
                SQLDescribeCol(hstmt, colCount, colName, sizeof(colName), 0, 0, 0, 0, 0);

                strcpy(name[colCount], (char *) colName);

                //printf("colCount=%d name[%d]=%s\n",colCount,colCount,name[colCount]);
            }
            for (colCount = 1; colCount <= colNum; colCount++) {
                //printf("colCount=%d name[%d]=%s\n",colCount,colCount,name[colCount]);
                if (SqlChange(table, keyName, key, name[colCount], va_arg(list, char*))) {}
                else {
                    n = 0;
                }
            }
            va_end(list);
        }
    }
    return n;
}

/**
 * @brief �޸�ָ�������ض���ĳһ���µ�ĳһ�е�ֵ
 * @param[in] table ����
 * @param[in] keyName ������
 * @param[in] key ��������
 * @param[in] colName ����
 * @param[in] value �޸ĺ��ֵ
 * @return �Ƿ��޸ĳɹ�
 * @retval 0 ʧ��
 * @retval 1 �ɹ�
 */
__attribute__((unused)) int SqlChange(char *table, char *keyName, char *key, char *colName, char *value) {
    char *str1 = link(link("update ", table), " set ");
    char *str2 = link(link(str1, colName), " = \'");
    char *str3 = link(link(str2, value), "\' where ");
    char *str4 = link(link(str3, keyName), " = \'");
    char *query = link(link(str4, key), "\'");
    if (SqlConn()) {
        //printf("SQL:%s\n",query);
        if (SqlExecute(query)) {
            SqlDisConn();
            printf("Change Succeeded!\n");
            return 1;
        } else {
            SqlDisConn();
            printf("Change Failed!\n");
            return 0;
        }
    } else {
        printf("Couldn't connect to %s.\n", szDSN);
        return 0;
    }
}

/**
 * @brief ����ĳ���ȫ����Ϣ
 * @param[in] table �ñ����
 * @param[out] values ÿһ�е�ȫ����Ϣ
 * @return �Ƿ��ѯ�ɹ�
 * @retval 0 ʧ��
 * @retval 1 �ɹ�
 */
__attribute__((unused)) int SqlFindAll(char *table, char values[128][128]) {
    SQLSMALLINT colNum = 0;//����
    SQLSMALLINT colCount = 0;//��ǰ��
    int n = 0;//ƫ������ÿ����һ�У����Ӷ�Ӧ�������
    char *query = link("select * from ", table);
    char chval[20][128];//��ʱ���������һ�н��
    if (SqlConn()) {
        printf("SQL:%s\n", query);
        if (SqlExecute(query)) {
            SQLNumResultCols(hstmt, &colNum);
            rc = SQLAllocStmt(hDbc, &hstmt);
            rc = SQLPrepare(hstmt, (SQLCHAR *) query, SQL_NTS);
            for (colCount = 1; colCount <= colNum; colCount++) {
                SQLBindCol(hstmt, colCount, SQL_C_CHAR, &chval[colCount], 128, 0);
            }
            rc = SQLExecute(hstmt);
            rc = SQLFetch(hstmt);
            while (SQL_SUCCEEDED(rc)) {
                for (colCount = 1; colCount <= colNum; colCount++) {
                    strcpy(values[colCount - 1 + n], chval[colCount]);
                }
                rc = SQLFetch(hstmt);
                n += colNum;
            };
            rc = SQLFreeStmt(hstmt, SQL_DROP);
            SqlDisConn();
            printf("Find Succeeded!\n");
            return 1;
        } else {
            SqlDisConn();
            printf("Find Failed!\n");
            return 0;
        }
    } else {
        printf("Couldn't connect to %s.\n", szDSN);
        return 0;
    }
}

/**
 * @brief ��ָ�����²���ĳ����ȫ����Ϣ
 * @param[in] table ����
 * @param[in] keyName ������
 * @param[in] key ��������
 * @param[out] values ȫ����Ϣ
 * @attention values����Ϣȫ����char*���ʹ洢�������Ҫ��ȡ������������ע�����ת��
 * @return �Ƿ��ѯ�ɹ�
 * @retval 0 ʧ��
 * @retval 1 �ɹ�
 */
__attribute__((unused)) int SqlFindRow(char *table, char *keyName, char *key, char values[128][128]) {
    SQLSMALLINT colNum = 0;//����
    SQLSMALLINT colCount = 0;//��ǰ��
    SQLCHAR colName[128] = {'\0'};//����
    char *str1 = "", *str2, *str3, *str4, *query;
    char name[20][128];//�����������
    char chval[20][128];//��ʱ���������һ�н��
    strcpy(name[0], "select ");
    if (SqlConn()) {
        if (TableConn(table)) {
            SQLNumResultCols(hstmt, &colNum);
            for (colCount = 1; colCount <= colNum; colCount++) {
                SQLDescribeCol(hstmt, colCount, colName, sizeof(colName), 0, 0, 0, 0, 0);
                strcpy(name[colCount], (char *) colName);
                if (colCount == 1) {
                    str1 = link(str1, name[colCount]);
                } else {
                    str1 = link(str1, link("\,", name[colCount]));
                }
            }
            str2 = link(link(name[0], str1), " from ");
            str3 = link(link(str2, table), " where ");
            str4 = link(link(str3, keyName), "=\'");
            query = link(link(str4, key), "\'");
            printf("SQL:%s\n", query);
            if (SqlExecute(query)) {
                for (colCount = 1; colCount <= colNum; colCount++) {
                    SQLBindCol(hstmt, colCount, SQL_C_CHAR, &chval[colCount], 128, 0);
                }
                rc = SQLFetch(hstmt);
                while (SQL_SUCCEEDED(rc)) {
                    for (colCount = 1; colCount <= colNum; colCount++) {
                        strcpy(values[colCount - 1], chval[colCount]);
                    }
                    rc = SQLFetch(hstmt);
                };
                rc = SQLFreeStmt(hstmt, SQL_DROP);
                SqlDisConn();
                printf("Find Succeeded!\n");
                return 1;
            } else {
                SqlDisConn();
                printf("Find Failed!\n");
                return 0;
            }
        }
    } else {
        printf("Couldn't connect to %s.\n", szDSN);
        return 0;
    }
}

/**
 * @brief ��ָ�����²���ĳ�е�ȫ����Ϣ
 * @param[in] table ����
 * @param[in] colName ����
 * @param[out] values ȫ����Ϣ
 * @attention values����Ϣȫ����char*���ʹ洢�������Ҫ��ȡ������������ע�����ת��
 * @return �Ƿ��ѯ�ɹ�
 * @retval 0 ʧ��
 * @retval 1 �ɹ�
 */
__attribute__((unused)) int SqlFindCol(char *table, char *colName, char values[128][128]) {
    SQLSMALLINT rowNum = 0;//����
    SQLSMALLINT rowCount = 0;//��ǰ��
    SQLSMALLINT colNum = 0;//����
    SQLSMALLINT colCount = 0;//��ǰ��
    //char name[128]={'\0'};
    char chval[128];//��ʱ���������δ��ÿ�н��
    char *str = link(link("select ", colName), " from ");
    char *query = link(str, table);
    if (SqlConn()) {
        if (TableConn(table)) {
            SQLNumResultCols(hstmt, &colNum);
            printf("SQL:%s\n", query);
            if (SqlExecute(query)) {
                SQLNumResultCols(hstmt, &colNum);
                rc = SQLAllocStmt(hDbc, &hstmt);
                rc = SQLPrepare(hstmt, (SQLCHAR *) query, SQL_NTS);
                SQLBindCol(hstmt, 1, SQL_C_CHAR, &chval, 128, 0);
                rc = SQLExecute(hstmt);
                rc = SQLFetch(hstmt);
                while (SQL_SUCCEEDED(rc)) {
                    for (colCount = 1; colCount <= colNum; colCount++) {
                        strcpy(values[colCount - 1 + rowCount], chval);
                    }
                    rc = SQLFetch(hstmt);
                    rowCount++;
                };
                rc = SQLFreeStmt(hstmt, SQL_DROP);
                SqlDisConn();
                printf("Find Succeeded!\n");
                return 1;
            } else {
                SqlDisConn();
                printf("Find Failed!\n");
                return 0;
            }
        }
    } else {
        printf("Couldn't connect to %s.\n", szDSN);
        return 0;
    }
}

/**
 * @brief ��ѯָ����������ĳһ��ָ�����µ�����
 * @param[in] table ����
 * @param[in] keyName ������
 * @param[in] key ��������
 * @param[in] colName ����
 * @param[out] value ���ҵ���Ϣ
 * @attention value����Ϣ��char*���ʹ��棬�����ȡ����������������ע��ת��
 * @return �Ƿ��ѯ�ɹ�
 * @retval 0 ʧ��
 * @retval 1 �ɹ�
 */
__attribute__((unused)) int SqlFind(char *table, char *keyName, char *key, char *colName, char *value) {
    SQLSMALLINT colNum = 0;//����
    SQLSMALLINT colCount = 0;//��ǰ��
    char chval[128];//��ʱ��Ų�ѯ���
    char *str1 = link(link("select ", colName), " from ");
    char *str2 = link(link(str1, table), " where ");
    char *str3 = link(link(str2, keyName), " =\'");
    char *query = link(link(str3, key), "\'");
    if (SqlConn()) {
        if (TableConn(table)) {
            SQLNumResultCols(hstmt, &colNum);
            printf("SQL:%s\n", query);
            if (SqlExecute(query)) {
                SQLBindCol(hstmt, 1, SQL_C_CHAR, &chval, 128, 0);
                rc = SQLFetch(hstmt);
                if (SQL_SUCCEEDED(rc)) {
                    strcpy(value, chval);
                    //rc = SQLFetch(hstmt);
                };
                rc = SQLFreeStmt(hstmt, SQL_DROP);
                SqlDisConn();
                printf("Find Succeeded!\n");
                return 1;
            } else {
                SqlDisConn();
                printf("Find Failed!\n");
                return 0;
            }
        }
    } else {
        printf("Couldn't connect to %s.\n", szDSN);
        return 0;
    }
}

/**
 * @brief Ϊָ�������һ��
 * @param[in] table ����
 * @param[in] colName ����
 * @return �Ƿ���ӳɹ�
 * @retval 0 ʧ��
 * @retval 1 �ɹ�
 */
__attribute__((unused)) int SqlAddCol(char *table, char *colName) {
    char *str = link(link("alter table ", table), " add ");
    char *query = link(link(str, colName), " varchar(100) ");
    if (SqlConn()) {
        printf("SQL:%s\n", query);
        if (SqlExecute(query)) {
            SqlDisConn();
            printf("Column Add Succeeded!\n");
            return 1;
        } else {
            SqlDisConn();
            printf("Column Add Failed!\n");
            return 0;
        }
    } else {
        printf("Couldn't connect to %s.\n", szDSN);
        return 0;
    }
}

/**
 * @brief Ϊָ����ɾ��ĳһ��
 * @param[in] table ����
 * @param[in] colName ����
 * @return �Ƿ�ɾ���ɹ�
 * @retval 0 ʧ��
 * @retval 1 �ɹ�
 */
__attribute__((unused)) int SqlDelCol(char *table, char *colName) {
    char *str = link(link("alter table ", table), " drop column ");
    char *query = link(str, colName);
    if (SqlConn()) {
        printf("SQL:%s\n", query);
        if (SqlExecute(query)) {
            SqlDisConn();
            printf("Column Delete Succeeded!\n");
            return 1;
        } else {
            SqlDisConn();
            printf("Column Delete Failed!\n");
            return 0;
        }
    } else {
        printf("Couldn't connect to %s.\n", szDSN);
        return 0;
    }
}


void main() {
    char value[20];
    char values[20][128];
    int i;

    //SqlAdd("student","S2222","hahfa","��","20","C2222");

    //SqlClear("teacher");

    //SqlDel("student","student_id","S1111");

    //SqlChange("student","student_id","S2222","student_name","avedh");

    //SqlChangeRow("student","student_id","S2222","S2222","svdkjb","��","23","C3333");

    //SqlChangeCol("student","age","99");

    //SqlAddCol("teacher","blbl");

    //SqlDelCol("teacher","blbl");

    /*SqlFindAll("student",values);
    for(i=0;i<20;i++){
        printf("%s ",values[i]);
    }printf("\n");*/

    /*SqlFindRow("student","student_id","S2222",values);
    for(i=0;i<5;i++){
        printf("%s ",values[i]);
    }printf("\n");*/

    /*SqlFindCol("student","student_name",values);
    for(i=0;i<5;i++){
        printf("%s ",values[i]);
    }printf("\n");*/


    /*if(SqlFind("student","student_id","S2222","student_name",value))
        printf("%s ",value);*/
    getchar();
}