# arduino-iambic-ab-keyer
Arduino based Iambic B / Iambic A keyer - radio telegraph manipulator controller for ham CW with additional TX out and programmable delay between TX and Key OUT

Автоматический телеграфный ключ с ямбическим режимом работы (A и B), а также дополнительным выходным сигналом TX для переключения трансивера в режим передачи. При этом он опережает сигнал выхода ключа Key OUT  на 50 мс, что бы все механические коммутаторы (рэле) трансивера и усилителя мощности переключились в нужное состояние. По завершении манипуляции через 0.8 сек сигнал TX обнуляется.
Для реализации проекта я использовал Arduino Nano из-за её малых размеров, но подойдет любая другая аналогичная или подобная плата. Нужно будет только внимательно отследить соответствие номеров ножек (“распиновку”) с их программным обозначением.

<b>Описание входов и выходов:</b><br>
D3 - вход Точка.<br>
D2 - вход Тире.<br>
<i>Примечание: Точки и тире замыкать на землю. Для инверсии поменять ножки местами.</i><br>
D4 - выход ключа Key OUT.<br>
D5 - выход TX.<br>
D11 - желтый светодиод Key OUT.<br>
D12 - красный светодиод TX.<br>
D8 - выход звукового сигнала самоконтроля.<br>
A0 - вход регулятора тона (потенциометр 20К).<br>
A1 - вход регулятора скорости (потенциометр 20К).<br>

<b>Iambic A и Iambic B:</b><br>
В интернете есть несколько статей, описывающих отличие режима A и B. Если говорить в двух словах, то режим B – это режим с памятью противоположного знака. Т.е. если во время формирования текущего знака коснуться противоположного и отпустить ключ, то по его завершении последует противоположный. В режиме A не последует. Лично мне комфортен режим Iambic B. Переключиться очень легко, поменяв в программе начальное значение переменной.<br>
yaType = 1; // Iambic B<br>
yaType = 0; // Iambic A<br>
