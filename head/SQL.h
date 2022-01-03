/**
 * @file SQL.h
 * @brief ����ͷ�ļ�
 * @details �������ӹ������ݿ����ص�ͷ�ļ�
 * @author gerivii
 * @date 2021-1-2
 * @version 1.2
 */


#ifndef STUDENTMANAGESYSTEM_SQL_H
#define STUDENTMANAGESYSTEM_SQL_H


/**
 * @brief �����ݿ�ָ�����������Ϣ
 * @param[in] table ����
 * @param[in] ... ��������ֵ����ĳ����Ϣ���������ö�Ӧ��Ĭ��ֵ���
 * @return �Ƿ�ɹ�
 * @retval 0    ʧ��
 * @retval 1    �ɹ�
 */
__attribute__((unused)) int SqlAdd(char *table, ...);


/**
 * @brief ɾ�����ݿ���ָ�����е�ĳһ��
 * @param[in] table ���ݿ���ָ���ı�
 * @param[in] keyName ������
 * @param[in] key ��������
 * @return �Ƿ�ɾ���ɹ�
 * @retval 0 ʧ��
 * @retval 1 �ɹ�
 */
__attribute__((unused)) int SqlDel(char *table, char *keyName, char *key);

/**
 * @brief ������ݿ���ָ����
 * @param[in] table ���ݿ���ָ���ı�
 * @return �Ƿ���ճɹ�
 * @retval 0 ʧ��
 * @retval 1 �ɹ�
 */
__attribute__((unused)) int SqlClear(char *table);

/**
 * @brief �޸�ָ������ĳһ����ȫ��ֵ
 * @param[in] table ����
 * @param[in] keyName ������
 * @param[in] key ��������
 * @param ... �޸ĺ��ֵ
 * @return �Ƿ��޸ĳɹ�
 * @retval 0 ʧ��
 * @retval 1 �ɹ�
 */
__attribute__((unused)) int SqlChangeRow(char *table, char *keyName, char *key,  ...);

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
__attribute__((unused)) int SqlChange(char *table, char *keyName, char *key, char *colName, char *value);

/**
 * @brief ����ĳ���ȫ����Ϣ
 * @param[in] table �ñ����
 * @param[in] keyName ������
 * @param[in] key ��������
 * @param[out] values ÿһ�е�ȫ����Ϣ
 * @return �Ƿ��ѯ�ɹ�
 * @retval 0 ʧ��
 * @retval 1 �ɹ�
 */
__attribute__((unused)) int SqlFindAll(char *table, char values[128][128]);

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
__attribute__((unused)) int SqlFindRow(char *table, char *keyName, char *key, char values[128][128]);

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
__attribute__((unused)) int SqlFindCol(char *table, char *colName, char values[128][128]);

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
__attribute__((unused)) int SqlFind(char *table, char *keyName, char *key, char *colName,char* value);

/**
 * @brief Ϊָ�������һ��
 * @param[in] table ����
 * @param[in] colName ����
 * @return �Ƿ���ӳɹ�
 * @retval 0 ʧ��
 * @retval 1 �ɹ�
 */
__attribute__((unused)) int SqlAddCol(char *table, char *colName);

/**
 * @brief Ϊָ����ɾ��ĳһ��
 * @param[in] table ����
 * @param[in] colName ����
 * @return �Ƿ�ɾ���ɹ�
 * @retval 0 ʧ��
 * @retval 1 �ɹ�
 */
__attribute__((unused)) int SqlDelCol(char *table, char *colName);

#endif //STUDENTMANAGESYSTEM_SQL_H
