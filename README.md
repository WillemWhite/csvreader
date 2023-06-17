# About
Программа была написана согласно ТЗ для тестового задания. Читает csv таблицу из *.csv файла и, при необходимости, находит значения функций, указанных в ячейках как:  
```= ARG1 OP ARG2```  
где ```ARG1``` и ```ARG2``` - адреса ячеек в таблице,   
а ```OP``` - одна из операций: ```{+, -, *, /}```  
  
Причем значения в ячейках - целые числа (расширено - хранить можно что угодно, но, если вычисляется значение функции, то ее операнды должны быть целыми числами).  
P.S. В моем случае ```/``` - целочисленное деление (ведь, раз храним целочисленные, то и тут тоже).
# Building
Для сборки программы используется CMake.   
Нужно произвести 2 команды в терминале из каталога проекта ($ - приглашение системы к вводу): 
``` bash
$ cmake ..
$ cmake --build . --config Release --target csvreader
```
Или же можно дать права на выполнение скрипта сборки под Linux и запустить через терминал:
``` bash
$ chmod +x build-csvreader.sh
$ ./build-csvreader.sh
```
# Running
Для запуска программы введите в командной строке Linux (или измените название программы в зависимости от ОС):
``` bash
$ ./csvreader file.csv
```
Редактируйте файл как пожелаете. Сообщения об ошибках были как можно более проработаны.
# Testing
Далее будет серия тестов для демонстрации возможностей работы программы. 

**- Пример 1 успешной работы программы:**  
<img src="tests-imgs/success-ex-1.png">

**- Пример 2 успешной работы программы:**  
<img src="tests-imgs/success-ex-2.png">  

**- Пример 3 успешной работы программы:**  
<img src="tests-imgs/success-ex-3.png">  

**- Пример 4 успешной работы программы(csv файл):**  
<img src="tests-imgs/success-ex-4.png">  

**- Пример 4 успешной работы программы(вывод программы):**  
<img src="tests-imgs/success-ex-4-message.png">

**- Пример 5 успешной работы программы:**  
<img src="tests-imgs/success-ex-5.png">  

**- Пример 1 ошибки структуры файла(csv файл):**  
<img src="tests-imgs/wrong-file-ex-1.png">

**- Пример 1 ошибки структуры файла(вывод):**  
<img src="tests-imgs/wrong-file-ex-1-message.png">

**- Пример 2 ошибки структуры файла(csv файл):**  
<img src="tests-imgs/wrong-file-ex-2.png">

**- Пример 2 ошибки структуры файла(вывод):**  
<img src="tests-imgs/wrong-file-ex-2-message.png">

**- Пример 3 ошибки структуры файла(csv файл):**  
<img src="tests-imgs/wrong-file-ex-3.png">

**- Пример 3 ошибки структуры файла(вывод):**  
<img src="tests-imgs/wrong-file-ex-3-message.png">

**- Пример 4 ошибки структуры файла(csv файл):**  
<img src="tests-imgs/wrong-file-ex-4.png">

**- Пример 4 ошибки структуры файла(вывод):**  
<img src="tests-imgs/wrong-file-ex-4-message.png">

**- Пример 5 ошибки структуры файла(csv файл):**  
<img src="tests-imgs/wrong-file-ex-5.png">

**- Пример 5 ошибки структуры файла(вывод):**  
<img src="tests-imgs/wrong-file-ex-5-message.png">

**- Пример 1 ошибки формулы в таблице:**  
<img src="tests-imgs/wrong-formula-ex-1.png">

**- Пример 2 ошибки формулы в таблице:**  
<img src="tests-imgs/wrong-formula-ex-2.png">

**- Пример 3 ошибки формулы в таблице:**  
<img src="tests-imgs/wrong-formula-ex-3.png">

**- Пример 4 ошибки формулы в таблице:**  
<img src="tests-imgs/wrong-formula-ex-4.png">

**- Пример 5 ошибки формулы в таблице:**  
<img src="tests-imgs/wrong-formula-ex-5.png">

**- Пример 6 ошибки формулы в таблице:**  
<img src="tests-imgs/wrong-formula-ex-6.png">

**- Пример 7 ошибки формулы в таблице:**  
<img src="tests-imgs/wrong-formula-ex-7.png">
