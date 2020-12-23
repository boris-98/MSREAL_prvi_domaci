# MSREAL_prvi_domaci

NAPOMENA ZA APLIKACIJU: S obzirom da je implementirano blokiranje pomocu semafora, 
                        ako se pokusa citanje praznog stringa ili se prekoraci STRING_SIZE pri konkatenaciji,
                        aplikacija ce se zablokirati.
                        Ukoliko se ove operacije izvode van aplikacije, uz pozadinsko izvrsavanje (sa &),
                        drajver ce ispravno funkcionisati.
