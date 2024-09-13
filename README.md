# Tema 3 Sd

Echipa tema:

rares.anton

viorel_cosmin.cucu

## Task 1

    Pentru a salva conexiunile intre utilizatori, folosim o matrice friends, alocata dinamic si ninitializata cu 0. Daca se adauga sau se sterge o conexiune intre doi utilizatori cu id1, respectiv id2, atunci elementele de pe pozitiile (id1,id2) si (id2,id1) devin 1, respectiv 0. Cu ajutorul acesteia, putem face diverse actiuni pentru un utilizator, precum sugestii de prieteni, numarul de prieteni, sau daca este sau nu cel mai popular dintre prietenii sai.

    > int distance_helper: avand date id-urile a doi utilizatori si matricea de adiacenta, calculam cea mai scurta distanta dintre ei 2 utilizatori. Pornind de la id1, il punem intr o coada queue. Dupa aceea introducem in coada toti prietenii lui id1, ii notam ca vizitati in alt vector visited, apoi il scoatem pe id1 si luam urmatorul element din coada. Repetam procesul pana se gaseste id2 sau pana nu mai sunt elemente in coada. Daca nu exista legatura intre id1 si id2, functia returneaza -1.

    > void popular_helper: pornind de la un utilizator, aflam care persoana este cea mai populara dni lista sa de prieteni, adica aflam ce utilizator are cei mai multi prieteni.


## Task 2

    Am creat o structura numita "node_post_t" ce reprezinta o postare/repostare ce continde id-ul ei, titlul, user_id, un arbore genric ce are ca root o postare, in rest fiind repostari(copii). Acest arbore il fac NULL pentru repostari. In acest nod mai am si o structura numita "like_t" ce contine un vector ce retine deca un utilizator a dat like sau nu si numarul de like-uri.
    Pe aceste postari le tin intr o lista de postari numita "posts_list".

    > void common_repost: scot cele 2 repostari din arbore apoi in structura unui nod din arbore retin si parintele acelui nod, astfel parcug calea de la repost la postare(root) si bag toti indexi reposturilor intr-un vector, iar de aici aflu trivial primul stramos comun

    > void delete_repost: vreau sa specific de ce returnez in funtia de free_coarbore un vector de vector de pointeri de g_node_t(g_node_t***). Deci eu caut repostarea cu acel id, facand o parcugere bfs eu o sa bag iN coada toti copii a unui nod la care ma aflu, iar in acest vector retin pointerul la vectorul de copii pentru ca eu nu ii pot elibera in functie deoarece o sa ma mai folosesc de ei si o sa le dau free dupa ce se termina functia.



## Task 3

    > Clica: Aflam pentru un nod cel mai mare subgraf complet. Pentru acest lucru, ne folosim de o metoda de backtracking. Initializam 2 vectori group si max_group. Functia number_nodes afla toate nodurile din graf. Functia max_clique implementeaza backtracking-ul: in max_group se vor stoca in final utilizatorii din clica, iar in max_size numarul utilizatori. vectorul group retine multimea utilizatorilor care trebuie verificata. Initial, group are doar id-ul utilizatorului stocat. Daca size este mai mare decat max_size, se actualizeaza max_goup. Verificam pentru fiecare alt nod daca formeaza cu grupul curent un subgraf complet. Daca se formeaza, se introduce nodul in group si se repeta procedeul. In final se obtine max_group cu clica dorita. Dupa ce se sorteaza crescator id urile din max_group, se afiseaza toti utilizatorii.
    Celelalte functii sunt prezentate destul de clar in cod.
