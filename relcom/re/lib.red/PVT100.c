/*
 * Таблица кодировки ввода/вывода для терминала
 * VT200, с загруженным русским алфавитом
 * Кодировка на вводе соответствует переводу ЙЦУКЕН - QUERTY
 * то есть изображает русскую клавиатуру на клавиатуре QUERTY.
 */
struct tablecod {
		  char ct_out[64];
		  char ct_in[96]; } tablecod =
{
   {    0300, 0301, 0302, 0303, 0304, 0305, 0306, 0307,
	0310, 0311, 0312, 0313, 0314, 0315, 0316, 0317,
	0320, 0321, 0322, 0323, 0324, 0325, 0326, 0327,
	0330, 0331, 0332, 0333, 0334, 0335, 0336, 0337,
	0340, 0341, 0342, 0343, 0344, 0345, 0346, 0347,
	0350, 0351, 0352, 0353, 0354, 0355, 0356, 0357,
	0360, 0361, 0362, 0363, 0364, 0365, 0366, 0367,
	0370, 0371, 0372, 0373, 0374, 0375, 0376, 0377},

  {
	' ', '!', 'Э', '?', '$', '%', '&',   'э',    /* 040 - 047 */
	'(', ')', '*', '+', ',', '-', '.',   'ю',    /* 050 - 057 */
	'0', '1', '2', '3', '4', '5', '6',   '7',    /* 060 - 067 */
	'8', '9', 'Ж', 'ж', 'б', '=', 'Б',   'Ю',    /* 070 - 077 */
	'/', 'Ф', 'И', 'С', 'В', 'У', 'А',   'П',    /* 100 - 107 */
	'Р', 'Ш', 'О', 'Л', 'Д', 'Ь', 'Т',   'Щ',    /* 110 - 117 */
	'З', 'Й', 'К', 'Ы', 'Е', 'Г', 'М',   'Ц',    /* 120 - 127 */
	'Ч', 'Н', 'Я', 'х', ',', ']', '^',   '_',    /* 130 - 137 */
	'`', 'ф', 'и', 'с', 'в', 'у', 'а',   'п',    /* 140 - 147 */
	'р', 'ш', 'о', 'л', 'д', 'ь', 'т',   'щ',    /* 150 - 157 */
	'з', 'й', 'к', 'ы', 'е', 'г', 'м',   'ц',    /* 160 - 167 */
	'ч', 'н', 'я', 'Х', '|', 'ъ', '~',   ' '     /* 170 - 177 */
  }
};


main()
{
exit(write(1,&tablecod,(sizeof tablecod)) != (sizeof tablecod));
}


