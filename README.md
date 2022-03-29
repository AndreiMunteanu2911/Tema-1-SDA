open_train_station
Functia aloca memorie pentru o structura de tip Train**, verifica daca alocarea a reusit si apoi atribuie valoarea primita in statie->platforms_no

close_train_station
Functia elibereaza memoria asociata unei structuri de tip TrainStation cu ajutorul functiei free

show_existing_trains
In urma unor verificari care se asigura daca pointerii primiti ca parametrii sunt corecti, functia printeza intr-un fisier nr tuturor peroanelor si greutatile vagoanelor respective cu ajutorul unui pointer copie

arrive_train
Verificam daca peronul este gol, si daca da alocam memorie pentru o structura de tip Train* si setam locomotive_power la valoarea primita

leave_train
In urma verificarilor daca parametrii primiti sunt corecti, eliberam memoria pentru fiecare vagon si setam locomotiva la NULL

add_train_car
Dupa verificarile de rigoare, alocam memorie pentru un element de tip TrainCar*, iar daca peronul primit nu are alte vagoane atunci il punem primul, altfel, cu ajutorul pointer ajungem la pozitia pe care trubuie sa inseram vagonul nou si il introducem in lista

remove_train_cars
Verificam daca parametrii primiti sunt corecti, iar apoi, prima data eliminam vagoanele cu greutatea primita care se afla la inceput, iar odata ce am ajuns la un vagon cu o greutate diferita folosim un pointer nou pentru a memora pozitia precedenta cat timp parcurgem lista de vagoane si cautam vagoane de greutatea dorita 

move_train_cars


find_express_train
Intr-un vector v alocat dinamic memoram pentru fiecare indice al platformei corespunzatoare diferenta dintre puterea locomotivei si suma greutatilor vagoanelor, si apoi comparam fiecare element din vector pentru a-l gasi pe cel mai mare, care semnifica trenul cel mai rapid

find_overload_train


find_optimal_train
Intr-un vector v alocat dinamic memoram pentru fiecare indice al platformei corespunzatoare diferenta dintre puterea locomotivei si suma greutatilor vagoanelor, si apoi comparam fiecare element din vector pentru a-l gasi pe cel mai mic, dar totodata mai mare ca 0, care semnifica trenul cel mai optim

find_heaviest_sequence_train


order_train
Dupa verificarile obisnuite, sortam lista de vagoane comparand fiecare valoare a greutatii acestora intre ele

fix_overload_train
