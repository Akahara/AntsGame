#include "CException.h"

EXPORT_C CException* CException_new(void) {
	return new CException();
}

EXPORT_C CException* CException_new_with_param(int value) {
	return new CException(value);
}

EXPORT_C void CException_delete(CException* exception) {
	delete exception;
}

EXPORT_C bool CException_getValeur(CException* exception) {
	return exception->getValeur();
}

EXPORT_C void CException_setValeur(CException* exception, int value) {
	exception->setValeur(value);
}
