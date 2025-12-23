#include <TechWWF.h>

char *BBSName, *BBSSysOp, *BBSID;
char *OutDir, *DoneString = { "" };

int ForceISO = 1;

char *UAlias, *UAddress, *UPostCode, *UCity, *UCountry, *UPhone;

struct CfgItem CfgList[] = {
    "BBSName", &BBSName, -2,
    "BBSSysOp", &BBSSysOp, -2,
    "BBSID", &BBSID, -2,
    "DoneMessage", &DoneString, -2,
    "TmpDir", &OutDir, -2,
    "UserAliasField", &UAlias, -2,
    "UserAddressField", &UAddress, -2,
    "UserPostalCodeField", &UPostCode, -2,
    "UserCityField", &UCity, -2,
    "UserCountryField", &UCountry, -2,
    "UserPhoneField", &UPhone, -2,
    "ForceISO", &ForceISO, 1,
    "NoForceISO", &ForceISO, 0,
    NULL, NULL, 0
};

int Quiet, TaskPri = 0;

char *pack = { "lha a" };

char *unpack = { "lha x" };

