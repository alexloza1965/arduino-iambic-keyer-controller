# arduino-iambic-ab-keyer
Arduino based Iambic B / Iambic A keyer - radio telegraph manipulator controller for ham CW with additional TX out and programmable delay between TX and Key OUT

Автоматический телеграфный ключ с ямбическим режимом работы (A и B), а также дополнительным выходным сигналом TX для переключения трансивера в режим передачи. При этом он опережает сигнал выхода ключа Key OUT  на 50 мс, что бы все механические коммутаторы (рэле) трансивера и усилителя мощности переключились в нужное состояние. По завершении манипуляции через 0.8 сек сигнал TX обнуляется.
