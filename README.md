# Pouzdan prenos preko UDP protokola
Projekat implementira **pouzdan prenos podataka** preko **UDP** protokola korišćenjem **Stop-and-Wait** mehanizma. Sistem koristi prilagođeno zaglavlje (custom header), potvrde o prijemu (**ACK**) i automatsku retransmisiju paketa u slučaju isteka vremenskog ograničenja (**timeout**).

## Build

Za kompajliranje pokrenuti makefile unutar `src` direktorijuma:

```bash
make
```



## Uputstvo za pokretanje 

### 1. Server
Iz `src` direktorijuma prvo pokrenuti server koji će čekati dolazne pakete:

```bash
./server
```

### 2. Klijent
Nakon što je server pokrenut, pokrenuti klijenta i navesti putanju do datoteke za slanje:

```bash
./client <naziv_datoteke> [ip_adresa]
```
**Napomena:** Opciono može se navesti IP adresa servera kao drugi argument. Ukoliko adresa nije navedena, klijent podrazumeva da se server nalazi na lokalnom računaru (**localhost**).




