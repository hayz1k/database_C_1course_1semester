#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS
#define COUNT 3

#include <stdio.h>
#include <math.h>
#include <locale.h>
#include <string.h>
#include <stdlib.h>


// Структура, содержащая данные о потребителе
struct person {
	int personal_account;         // Лицевой счет (в виде номера пользователя по типу id)
	char surname[15];             // Фамилия
	int current_counter[12];      // Текущее показание счетчика (по месяцам)
	float for_payment[12];        // Начислено к оплате (по месяцам)
	float payment[12];            // Сумма оплаты (по месяцам)
	float sr;                     // Средняя сумма оплаты (за 3 месяца)
	float for_payment_sum;        // Общая сумма для оплаты (за 3 месяца)
	float payment_sum;            // Общая сумма оплаты (за 3 месяца)
	float debt_sum;				  // Общая сумма долга
};

typedef struct person data_t;

void menu();
int surname_search(data_t* ptr_struct, int size);
void tabulation(data_t* ptr_struct, int i);
void full_tabulation(data_t* ptr_struct, int size);
void sum_tabulation(data_t* ptr_struct, int size);
int add_data(data_t* ptr_struct, int size);
int compare_paymentname(const data_t* av, const data_t* bv);
void sort(data_t* ptr_struct, int size);
int readfile(data_t** ptr_struct, int size);
int writefile(data_t* ptr_struct, int size);
int payment_search(data_t* ptr_struct, int size);



void main() {
	setlocale(LC_ALL, "RUS");
	int x, y, size = 0, end = 0, choice1 = 0, choice2 = 0;
	char search_surname[20];

	data_t* ptr_struct = (struct data_t*)malloc(COUNT * sizeof(data_t));

	while (end != 8) {
		menu(); // Вызываем функцию табуляции меню в консоль;
		scanf("%d", &end);

		switch (end) {

		case 1: // Ввести данные о потребителе
			system("cls");
			size = add_data(ptr_struct, size);
			ptr_struct = (data_t*)realloc(ptr_struct, sizeof(data_t) * (size + 1));
			break;

		case 2: // Найти потребителя по сумме оплаты
			// Переменная <x> хранит в себе индекс пользователя, найденного по сумме оплаты,
			// или же 999 в случае если такого пользователя нет
			system("cls");
			puts("1. Поиск по общей сумме оплаты");
			puts("2. Поиск по оплате за какой-либо месяц");
			scanf("%d", &choice1);
			switch (choice1) {
			case 1:
				x = payment_sum_search(ptr_struct, size);
				if (x != 999)
					tabulation(ptr_struct, x);
				else
					printf("Пользователь не найден\n");
				break;
			case 2:
				x = payment_search(ptr_struct, size);
				if (x != 999)
					tabulation(ptr_struct, x);
				else
					printf("Пользователь не найден\n");
				break;
			}
			break;

		case 3: // Найти потребителя по фамилии
			system("cls");
			// Переменная <y> хранит в себе индекс пользователя, найденного по фамилии,
			// или же 999 в случае если такого пользователя нет
			y = surname_search(ptr_struct, size);
			if (y != 999)
				tabulation(ptr_struct, y);
			else
				printf("Пользователь не найден\n");
			break;

		case 4: // Отсортировать список структур по критерию (сумма оплаты)
			system("cls");
			sort(ptr_struct, size);
			break;

		case 5: // Вывод информации о всех потребителях
			puts("1. Информация об оплате всех потребителей");
			puts("2. Информация об общей сумме оплаты/для оплаты/долга, средней сумме оплаты");
			scanf("%d", &choice2);
			switch (choice2) {
			case 1:
				full_tabulation(ptr_struct, size);
				break;
			case 2:
				sum_tabulation(ptr_struct, size);
				break;
			}
			break;
		case 6: // Запись в файл
			system("cls");
			writefile(ptr_struct, size);
			break;

		case 7: // Чтение с файла
			system("cls");
			size = readfile(&ptr_struct, size);
			ptr_struct = (data_t*)realloc(ptr_struct, sizeof(data_t) * (size + 1));
			break;

		case 8: // Выход из программы
			break;
		}
	}
}

// Подпрограмма для функции qsort, которая сравнивает два элемента структуры и возвращает значение, показывающее, как они соотносятся
int compare_paymentname(const data_t* av, const data_t* bv) {
	if (av->sr < bv->sr) return -1;
	if (av->sr > bv->sr) return 1;
	return 0;
}

// Сортировка по средней сумме оплаты
void sort(data_t* ptr_struct, int size) {
	// base, number, width, compare	
	qsort(ptr_struct, size, sizeof(data_t), compare_paymentname);
	printf("Сортировка завершена\n");
}

// Вывод меню на экран
void menu() {
	puts("-------------------МЕНЮ--------------------");
	puts("Выберите команду:");
	puts("1. Ввести данные о потребителях");
	puts("2. Найти потребителя по сумме оплаты");
	puts("3. Найти потребителя по фамилии");
	puts("4. Отсортировать по средней сумме оплаты");
	puts("5. Вывести данные о потребителях");
	puts("6. Запись в файл");
	puts("7. Чтение с файла");
	puts("8. Выход из программы");
	puts("-------------------------------------------");
}

// Поиск потребителя по критерию (по общей сумме оплаты)
int payment_sum_search(data_t* ptr_struct, int size) {
	float search_payment;
	printf("Введите сумму оплаты : ");
	scanf("%f", &search_payment);
	for (int i = 0; i < size; i++) {
		// Взял "прощаемую" погрешность в 0.1 рубль для сравнения float
		if (fabsf(ptr_struct[i].payment_sum - search_payment) < 0.1) {
			return i;
		}
	}
	// При несовпадении суммы оплаты ни с одним потребителем возвращаем 999 для обработки ошибок
	return 999;
}

// Поиск по оплате за какой-то месяц
int payment_search(data_t* ptr_struct, int size) {
	float payment;
	printf("Введите сумму оплаты : ");
	scanf("%f", &payment);
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < 12; j++) {
			if (fabsf(ptr_struct[i].payment[j] - payment) < 0.1) {
				return i;
			}
		}
		// Взял "прощаемую" погрешность в 0.1 рубль для сравнения float
	}
	// При несовпадении суммы оплаты ни с одним потребителем возвращаем 999 для обработки ошибок
	return 999;
}

// Поиск потребителя по критерию (по фамилии)
int surname_search(data_t* ptr_struct, int size) {
	char search_surname[15];
	printf("Введите фамилию потребителя : \n");
	scanf("%s", &search_surname);
	for (int i = 0; i < size; i++) {
		// strcmp сравнивает (str1, str2) побайтно, при полном совпадении возвращает 0
		// далее если функция все таки вернет нам 0, мы возвращаем индекс потребителя для дальнейшего вывода
		if (strcmp(ptr_struct[i].surname, search_surname) == 0)
			return i;
	}
	// При несовпадении фамилии ни с одним потребителем возвращаем 999 для обработки ошибок
	return 999;
}

// Табуляция информации о потребителе на экран
void tabulation(data_t* ptr_struct, int i) {
	printf("-----------------Потребитель #%d---------------------\n", ptr_struct[i].personal_account);
	printf("Лицевой счет: %d\n", ptr_struct[i].personal_account);
	printf("Фамилия: %s\n", ptr_struct[i].surname);

	printf("Показания счетчика: \n");
	for (int j = 0; j < 12; j++) {
		printf("   Месяц %d: %d\n", j + 1, ptr_struct[i].current_counter[j]);
	}

	printf("Начислено к оплате:\n");
	for (int j = 0; j < 12; j++) {
		printf("   Месяц %d: %.2f\n", j + 1, ptr_struct[i].for_payment[j]);
	}

	printf("Сумма оплаты: \n");
	for (int j = 0; j < 12; j++) {
		printf("   Месяц %d: %.2f\n", j + 1, ptr_struct[i].payment[j]);
	}
	printf("\nСредняя сумма оплаты = %.2f\n", ptr_struct[i].sr);
	printf("Задолженность = %.2f\n", ptr_struct[i].debt_sum);
	printf("Необходимая для оплаты общая сумма %.2f руб.\n", ptr_struct[i].for_payment_sum);
	printf("Общая сумма оплаты %.2f руб.\n", ptr_struct[i].payment_sum);
	printf("-----------------------------------------------------\n");
}

// Табуляции информации о потребителях на экран в виде таблицы
void full_tabulation(data_t* ptr_struct, int size) {
	printf("|---------|-------------------|-------|-----------|-----------|-------|-----------|-----------|-------|-----------|-----------|-------|-----------|-----------|\n");
	printf("| Лицевой |     Фамилия       | Месяц | Показания |   Сумма   | Месяц | Показания |   Сумма   | Месяц | Показания |   Сумма   | Месяц | Показания |   Сумма   |\n");
	printf("|    счет |                   |       |  счетчика |   оплаты  |       |  счетчика |   оплаты  |       |  счетчика |   оплаты  |       |  счетчика |   оплаты  |\n");
	printf("|---------|-------------------|-------|-----------|-----------|-------|-----------|-----------|-------|-----------|-----------|-------|-----------|-----------|\n");
	for (int i = 0; i < size; i++) {
		printf("|         |                   |	  1   | %5d     | %8.2f  |   4   | %5d     | %8.2f  |	  7   | %5d     | %8.2f  |  10   | %5d     | %8.2f  |\n",
			ptr_struct[i].current_counter[0], ptr_struct[i].payment[0], ptr_struct[i].current_counter[3], ptr_struct[i].payment[3],
			ptr_struct[i].current_counter[6], ptr_struct[i].payment[6], ptr_struct[i].current_counter[9], ptr_struct[i].payment[9]);
		printf("|         |                   |-------|-----------|-----------|-------|-----------|-----------|-------|-----------|-----------|-------|-----------|-----------|\n");

		printf("|  %3d    | %15s   |	  2   | %5d     | %8.2f  |   5   | %5d     | %8.2f  |	  8   | %5d     | %8.2f  |  11   | %5d     | %8.2f  |\n",
			ptr_struct[i].personal_account, ptr_struct[i].surname,
			ptr_struct[i].current_counter[1], ptr_struct[i].payment[1], ptr_struct[i].current_counter[2], ptr_struct[i].payment[2],
			ptr_struct[i].current_counter[7], ptr_struct[i].payment[7], ptr_struct[i].current_counter[8], ptr_struct[i].payment[8]);
		printf("|         |                   |-------|-----------|-----------|-------|-----------|-----------|-------|-----------|-----------|-------|-----------|-----------|\n");



		printf("|         |                   |	  3   | %5d     | %8.2f  |   6   | %5d     | %8.2f  |	  9   | %5d     | %8.2f  |  12   | %5d     | %8.2f  |\n",
			ptr_struct[i].current_counter[2], ptr_struct[i].payment[2], ptr_struct[i].current_counter[5], ptr_struct[i].payment[5],
			ptr_struct[i].current_counter[8], ptr_struct[i].payment[8], ptr_struct[i].current_counter[11], ptr_struct[i].payment[11]);
		printf("|---------|-------------------|-------|-----------|-----------|-------|-----------|-----------|-------|-----------|-----------|-------|-----------|-----------|\n");
	}


}

void sum_tabulation(data_t* ptr_struct, int size) {
	printf("|---------|-------------------|---------------|-------------|-------------|-------------|\n");
	printf("| Лицевой |     Фамилия       | Средняя сумма | Общая сумма | Общая сумма | Общая сумма |\n");
	printf("|    счет |                   |     оплаты    |  для оплаты |    оплаты   |   долга     |\n");
	printf("|---------|-------------------|---------------|-------------|-------------|-------------|\n");
	for (int i = 0; i < size; i++) {
		printf("| %3d     | %15s   | %11.1f   | %11.1f | %11.1f | %11.1f |\n", ptr_struct[i].personal_account, ptr_struct[i].surname,
			ptr_struct[i].sr, ptr_struct[i].for_payment_sum, ptr_struct[i].payment_sum, ptr_struct[i].debt_sum);
		printf("|---------|-------------------|---------------|-------------|-------------|-------------|\n");
	}
}

// Запись данных о потребителях
int add_data(data_t* ptr_struct, int size) {
	float tarif = 5.76; // Возьмем тариф за единицу показаний счетчика = 5.76 рублей
	float f_p_s = 0, p_s = 0, d_s = 0;
	printf("Введите номер лицевого счета: ");
	scanf("%d", &ptr_struct[size].personal_account);

	printf("Введите фамилию: ");
	scanf("%15s", &ptr_struct[size].surname);

	printf("Введите показания счетчика: \n");
	// В цикле запрашиваем ввод с клавиатуры показание счетчика за определенынй месяц
	for (int j = 0; j < 12; j++) {
		printf("Месяц %d: \n", j + 1);
		scanf("%d", &ptr_struct[size].current_counter[j]);
	}

	printf("Начислено к оплате: \n");
	// В цикле подсчитываем сумму денег, начисленную к оплате за определенный месяц
	for (int j = 0; j < 12; j++) {
		ptr_struct[size].for_payment[j] = tarif * ptr_struct[size].current_counter[j];
		printf("Месяц %d: %f руб.\n", j + 1, ptr_struct[size].for_payment[j]);
	}

	printf("Сумма оплаты: \n");
	// В цикле запрашиваем ввод с клавиатуры сумму оплаты за определенный месяц
	for (int j = 0; j < 12; j++) {
		printf("Месяц %d: \n", j + 1);
		scanf("%f", &ptr_struct[size].payment[j]);
	}

	for (int j = 0; j < 12; j++)
		f_p_s += ptr_struct[size].for_payment[j];
	for (int j = 0; j < 12; j++)
		p_s += ptr_struct[size].payment[j];
	for (int j = 0; j < 12; j++)
		d_s += ptr_struct[size].for_payment[j] - ptr_struct[size].payment[j];

	ptr_struct[size].debt_sum = d_s;
	ptr_struct[size].for_payment_sum = f_p_s;
	ptr_struct[size].payment_sum = p_s;
	ptr_struct[size].sr = ptr_struct[size].payment_sum / 12;
	return size + 1;
}

// Чтение данных с файла
int readfile(data_t** ptr_struct, int size) {
	int i = size - 1;
	FILE* ptr_file = fopen("test.txt", "r");
	if (ptr_file == NULL) {
		puts("Ошибка открытия файла для чтения");
		return size;
	}
	while (!feof(ptr_file)) {
		i++;
		*ptr_struct = (data_t*)realloc(*ptr_struct, sizeof(data_t) * (i + 1));
		fscanf(ptr_file, "%d %s %d %d %d %d %d %d %d %d %d %d %d %d %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f\n",
			&(*ptr_struct)[i].personal_account, &(*ptr_struct)[i].surname, &(*ptr_struct)[i].current_counter[0],
			&(*ptr_struct)[i].current_counter[1], &(*ptr_struct)[i].current_counter[2], &(*ptr_struct)[i].current_counter[3],
			&(*ptr_struct)[i].current_counter[4], &(*ptr_struct)[i].current_counter[5], &(*ptr_struct)[i].current_counter[6],
			&(*ptr_struct)[i].current_counter[7], &(*ptr_struct)[i].current_counter[8], &(*ptr_struct)[i].current_counter[9],
			&(*ptr_struct)[i].current_counter[10], &(*ptr_struct)[i].current_counter[11],

			&(*ptr_struct)[i].for_payment[0], &(*ptr_struct)[i].for_payment[1],
			&(*ptr_struct)[i].for_payment[2], &(*ptr_struct)[i].for_payment[3], &(*ptr_struct)[i].for_payment[4],
			&(*ptr_struct)[i].for_payment[5], &(*ptr_struct)[i].for_payment[6], &(*ptr_struct)[i].for_payment[7],
			&(*ptr_struct)[i].for_payment[8], &(*ptr_struct)[i].for_payment[9], &(*ptr_struct)[i].for_payment[10],
			&(*ptr_struct)[i].for_payment[11],

			&(*ptr_struct)[i].payment[0], &(*ptr_struct)[i].payment[1], &(*ptr_struct)[i].payment[2], &(*ptr_struct)[i].payment[3], &(*ptr_struct)[i].payment[4], &(*ptr_struct)[i].payment[5],
			&(*ptr_struct)[i].payment[6], &(*ptr_struct)[i].payment[7], &(*ptr_struct)[i].payment[8], &(*ptr_struct)[i].payment[9], &(*ptr_struct)[i].payment[10], &(*ptr_struct)[i].payment[11],

			&(*ptr_struct)[i].sr,
			&(*ptr_struct)[i].debt_sum, &(*ptr_struct)[i].for_payment_sum,
			&(*ptr_struct)[i].payment_sum);
	}
	fclose(ptr_file);
	printf("Чтение данных с файла завершено\n");
	return i + 1;
}

// Запись данных
int writefile(data_t* ptr_struct, int size) {
	FILE* ptr_file = fopen("test.txt", "w");
	if (ptr_file == NULL) {
		puts("Ошибка открытия файла для записи");
		return 0;
	}
	for (int i = 0; i < size; i++) {
		fprintf(ptr_file, "%d %s %d %d %d %d %d %d %d %d %d %d %d %d %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f\n",
			ptr_struct[i].personal_account, ptr_struct[i].surname, ptr_struct[i].current_counter[0],
			ptr_struct[i].current_counter[1], ptr_struct[i].current_counter[2], ptr_struct[i].current_counter[3],
			ptr_struct[i].current_counter[4], ptr_struct[i].current_counter[5], ptr_struct[i].current_counter[6],
			ptr_struct[i].current_counter[7], ptr_struct[i].current_counter[8], ptr_struct[i].current_counter[9],
			ptr_struct[i].current_counter[10], ptr_struct[i].current_counter[11],

			ptr_struct[i].for_payment[0], ptr_struct[i].for_payment[1],
			ptr_struct[i].for_payment[2], ptr_struct[i].for_payment[3], ptr_struct[i].for_payment[4],
			ptr_struct[i].for_payment[5], ptr_struct[i].for_payment[6], ptr_struct[i].for_payment[7],
			ptr_struct[i].for_payment[8], ptr_struct[i].for_payment[9], ptr_struct[i].for_payment[10],
			ptr_struct[i].for_payment[11],

			ptr_struct[i].payment[0], ptr_struct[i].payment[1], ptr_struct[i].payment[2], ptr_struct[i].payment[3], ptr_struct[i].payment[4], ptr_struct[i].payment[5],
			ptr_struct[i].payment[6], ptr_struct[i].payment[7], ptr_struct[i].payment[8], ptr_struct[i].payment[9], ptr_struct[i].payment[10], ptr_struct[i].payment[11],

			ptr_struct[i].sr,
			ptr_struct[i].debt_sum, ptr_struct[i].for_payment_sum,
			ptr_struct[i].payment_sum);
	}
	fclose(ptr_file);
	return size;
}
