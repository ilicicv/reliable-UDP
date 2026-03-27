# Pouzdan prenos preko UDP protokola

## Build

Za kompajliranje pokrenuti makefile unutar `src` direktorijuma:

```bash
make
```



## Pokretanje 

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
Ukoliko IP adresa nije navedena, klijent smatra da se server nalazi na lokalnom računaru.




