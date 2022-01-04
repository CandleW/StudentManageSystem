Studen#include "../head/Student.h"
#include "../head/SQL.h"

#include <string.h>

__attribute__((unused)) int StudentAdd(char* student_id, char* student_name, char* sex, int age, char* class_id) {
	return SqlAdd("student", student_id, student_name, sex, age, class_id);
}

__attribute__((unused)) int StudentDel(char* student_id) {
	return SqlDel("student", "student_id", student_id);
}

__attribute__((unused)) int StudentEdit(char* student_id, char* student_name, char* sex, int age, char* class_id) {
	return SqlChangeRow("student", "student_id", student_id, student_name, sex, age, class_id);
}

__attribute__((unused)) int StudentEditName(char* student_id, char* student_name) {
	return SqlChange("student", "student_id", student_id, "student_name", student_name);
}

__attribute__((unused)) int StudentEditSex(char* student_id, char* sex) {
	return SqlChange("student", "student_id", student_id, "sex", sex);
}

__attribute__((unused)) int StudentEditAge(char* student_id, int age) {
	return SqlChange("student", "student_id", student_id, "age", age);
}

__attribute__((unused)) int StudentEditClassId(char* student_id, char* class_id) {
	return SqlChange("student", "student_id", student_id, "class_id", class_id);
}

__attribute__((unused)) int StudentFind(char* student_id, char* student_name, char* sex, int age, char* class_id) {
	char values[20][128];
	if (!SqlFindRow("student", "student_id", student_id, values))
		return 0;
	strcpy(name, values[1]);
	strcpy(sex, values[2]);
	strcpy(age, values[3]);
	strcpy(class_id, values[4]);
	return 1;
}
