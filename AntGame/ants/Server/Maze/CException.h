#pragma once

#ifdef __cplusplus // If we use C++

class CException {

private:
	int valeur;

public:

	// Constructeurs :

	/*
	Constructeur par d�fault
	*/
	CException() {
		valeur = 0;
	}

	/*
	Constructeur de confort
	*/
	CException(unsigned int valeurIn) {
		valeur = valeurIn;
	}

public:

	//Accesseurs :

	/*
	Getter de l'atribut valeur
	*/
	const int getValeur() {
		return valeur;
	}

	/*
	Setter de l'atribut valeur
	*/
	void setValeur(int valeur) {
		valeur = valeur;
	}
};


#else // If we use C

typedef struct CException {
	int valeur
} CException;

#endif


// access functions
#ifdef __cplusplus
    #define EXPORT_C extern "C"
#else
    #define EXPORT_C
#endif


EXPORT_C CException* CException_new(void);

EXPORT_C CException* CException_new_with_param(int value);

EXPORT_C void CException_delete(CException* exception);

EXPORT_C bool CException_getValeur(CException* exception);

EXPORT_C void CException_setValeur(CException* exception, int value);