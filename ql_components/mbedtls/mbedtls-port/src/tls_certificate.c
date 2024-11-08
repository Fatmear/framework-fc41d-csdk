/*
 *  Copyright (C) 2006-2015, ARM Limited, All Rights Reserved
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  This file is part of mbed TLS (https://tls.mbed.org)
 */

#include "certs.h"

#if 0
#define ENTRUST_ROOT_CA                                                 \
"-----BEGIN CERTIFICATE-----\r\n"                                       \
"MIIEPjCCAyagAwIBAgIESlOMKDANBgkqhkiG9w0BAQsFADCBvjELMAkGA1UEBhMC\r\n"  \
"VVMxFjAUBgNVBAoTDUVudHJ1c3QsIEluYy4xKDAmBgNVBAsTH1NlZSB3d3cuZW50\r\n"  \
"cnVzdC5uZXQvbGVnYWwtdGVybXMxOTA3BgNVBAsTMChjKSAyMDA5IEVudHJ1c3Qs\r\n"  \
"IEluYy4gLSBmb3IgYXV0aG9yaXplZCB1c2Ugb25seTEyMDAGA1UEAxMpRW50cnVz\r\n"  \
"dCBSb290IENlcnRpZmljYXRpb24gQXV0aG9yaXR5IC0gRzIwHhcNMDkwNzA3MTcy\r\n"  \
"NTU0WhcNMzAxMjA3MTc1NTU0WjCBvjELMAkGA1UEBhMCVVMxFjAUBgNVBAoTDUVu\r\n"  \
"dHJ1c3QsIEluYy4xKDAmBgNVBAsTH1NlZSB3d3cuZW50cnVzdC5uZXQvbGVnYWwt\r\n"  \
"dGVybXMxOTA3BgNVBAsTMChjKSAyMDA5IEVudHJ1c3QsIEluYy4gLSBmb3IgYXV0\r\n"  \
"aG9yaXplZCB1c2Ugb25seTEyMDAGA1UEAxMpRW50cnVzdCBSb290IENlcnRpZmlj\r\n"  \
"YXRpb24gQXV0aG9yaXR5IC0gRzIwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEK\r\n"  \
"AoIBAQC6hLZy254Ma+KZ6TABp3bqMriVQRrJ2mFOWHLP/vaCeb9zYQYKpSfYs1/T\r\n"  \
"RU4cctZOMvJyig/3gxnQaoCAAEUesMfnmr8SVycco2gvCoe9amsOXmXzHHfV1IWN\r\n"  \
"cCG0szLni6LVhjkCsbjSR87kyUnEO6fe+1R9V77w6G7CebI6C1XiUJgWMhNcL3hW\r\n"  \
"wcKUs/Ja5CeanyTXxuzQmyWC48zCxEXFjJd6BmsqEZ+pCm5IO2/b1BEZQvePB7/1\r\n"  \
"U1+cPvQXLOZprE4yTGJ36rfo5bs0vBmLrpxR57d+tVOxMyLlbc9wPBr64ptntoP0\r\n"  \
"jaWvYkxN4FisZDQSA/i2jZRjJKRxAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAP\r\n"  \
"BgNVHRMBAf8EBTADAQH/MB0GA1UdDgQWBBRqciZ60B7vfec7aVHUbI2fkBJmqzAN\r\n"  \
"BgkqhkiG9w0BAQsFAAOCAQEAeZ8dlsa2eT8ijYfThwMEYGprmi5ZiXMRrEPR9RP/\r\n"  \
"jTkrwPK9T3CMqS/qF8QLVJ7UG5aYMzyorWKiAHarWWluBh1+xLlEjZivEtRh2woZ\r\n"  \
"Rkfz6/djwUAFQKXSt/S1mja/qYh2iARVBCuch38aNzx+LaUa2NSJXsq9rD1s2G2v\r\n"  \
"1fN2D807iDginWyTmsQ9v4IbZT+mD12q/OWyFcq1rca8PdCE6OoGcrBNOTJ4vz4R\r\n"  \
"nAuknZoh8/CbCzB428Hch0P+vGOaysXCHMnHjf87ElgI5rY97HosTvuDls4MPGmH\r\n"  \
"VHOkc8KT/1EQrBVUAdj8BbGJoX90g5pJ19xOe4pIb4tF9g==\r\n"                  \
"-----END CERTIFICATE-----\r\n"


const char mbedtls_root_certificate[] = 
	ENTRUST_ROOT_CA
	;
#elif 0
/////www.baidu.com
#define GLOBALSIGN_ROOT_CA                                                 \ 
"-----BEGIN CERTIFICATE-----\r\n"                                       \
"MIIDdTCCAl2gAwIBAgILBAAAAAABFUtaw5QwDQYJKoZIhvcNAQEFBQAwVzELMAkG\r\n"  \
"A1UEBhMCQkUxGTAXBgNVBAoTEEdsb2JhbFNpZ24gbnYtc2ExEDAOBgNVBAsTB1Jv\r\n"  \
"b3QgQ0ExGzAZBgNVBAMTEkdsb2JhbFNpZ24gUm9vdCBDQTAeFw05ODA5MDExMjAw\r\n"  \
"MDBaFw0yODAxMjgxMjAwMDBaMFcxCzAJBgNVBAYTAkJFMRkwFwYDVQQKExBHbG9i\r\n"  \
"YWxTaWduIG52LXNhMRAwDgYDVQQLEwdSb290IENBMRswGQYDVQQDExJHbG9iYWxT\r\n"  \
"aWduIFJvb3QgQ0EwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDaDuaZ\r\n"  \
"jc6j40+Kfvvxi4Mla+pIH/EqsLmVEQS98GPR4mdmzxzdzxtIK+6NiY6arymAZavp\r\n"  \
"xy0Sy6scTHAHoT0KMM0VjU/43dSMUBUc71DuxC73/OlS8pF94G3VNTCOXkNz8kHp\r\n"  \
"1Wrjsok6Vjk4bwY8iGlbKk3Fp1S4bInMm/k8yuX9ifUSPJJ4ltbcdG6TRGHRjcdG\r\n"  \
"snUOhugZitVtbNV4FpWi6cgKOOvyJBNPc1STE4U6G7weNLWLBYy5d4ux2x8gkasJ\r\n"  \
"U26Qzns3dLlwR5EiUWMWea6xrkEmCMgZK9FGqkjWZCrXgzT/LCrBbBlDSgeF59N8\r\n"  \
"9iFo7+ryUp9/k5DPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNVHRMBAf8E\r\n"  \
"BTADAQH/MB0GA1UdDgQWBBRge2YaRQ2XyolQL30EzTSo//z9SzANBgkqhkiG9w0B\r\n"  \
"AQUFAAOCAQEA1nPnfE920I2/7LqivjTFKDK1fPxsnCwrvQmeU79rXqoRSLblCKOz\r\n"  \
"yj1hTdNGCbM+w6DjY1Ub8rrvrTnhQ7k4o+YviiY776BQVvnGCv04zcQLcFGUl5gE\r\n"  \
"38NflNUVyRRBnMRddWQVDf9VMOyGj/8N7yy5Y0b2qvzfvGn9LhJIZJrglfCm7ymP\r\n"  \
"AbEVtQwdpf5pLGkkeB6zpxxxYu7KyJesF12KwvhHhm4qxFYxldBniYUr+WymXUad\r\n"  \
"DKqC5JlR3XC321Y9YeRq4VzW9v493kHMB65jUr9TU/Qr6cf9tveCX4XSQRjbgbME\r\n"  \
"HMUfpIBvFSDJ3gyICh3WZlXi/EjJKSZp4A==\r\n"                              \
"-----END CERTIFICATE-----"

const char mbedtls_root_certificate[] = 
	GLOBALSIGN_ROOT_CA
;
#elif 1
////my ca for mqtt
#define GLOBALSIGN_ROOT_CA   \
"-----BEGIN CERTIFICATE-----\r\n"\
"MIIE5TCCA02gAwIBAgIJAP5uWtPRe+IxMA0GCSqGSIb3DQEBCwUAMIGHMQswCQYD\r\n"\
"VQQGEwJDTjELMAkGA1UECAwCQUgxCzAJBgNVBAcMAkhGMRwwGgYDVQQKDBNEZWZh\r\n"\
"dWx0IENvbXBhbnkgTHRkMRgwFgYDVQQDDA8yMjAuMTgwLjIzOS4yMTIxJjAkBgkq\r\n"\
"hkiG9w0BCQEWF2VkZGllLnpoYW5nQHF1ZWN0ZWwuY29tMCAXDTIxMDEyNzAzMzk0\r\n"\
"M1oYDzIxMjEwMTAzMDMzOTQzWjCBhzELMAkGA1UEBhMCQ04xCzAJBgNVBAgMAkFI\r\n"\
"MQswCQYDVQQHDAJIRjEcMBoGA1UECgwTRGVmYXVsdCBDb21wYW55IEx0ZDEYMBYG\r\n"\
"A1UEAwwPMjIwLjE4MC4yMzkuMjEyMSYwJAYJKoZIhvcNAQkBFhdlZGRpZS56aGFu\r\n"\
"Z0BxdWVjdGVsLmNvbTCCAaIwDQYJKoZIhvcNAQEBBQADggGPADCCAYoCggGBANf3\r\n"\
"w0ep+Sv4qDjhafwc3wiaGdzwAXOsRgfGgBEGYL5MuPbpzjERo21yHae6Tx19DIUt\r\n"\
"g6hgW5N/bvDqXLbEMChy9b8aNl62+Y6O35Tya2hoz4XXQkrPkRXvlP2eonjPMfQB\r\n"\
"Yu4wtAfNKPa10MJF8qjFI7WWBpcQMpoQvt/SekoD6IyxgRu8ApjtmJicvvWu3BqW\r\n"\
"W93MZqLhSFf57pKLQEhgskpl/gb+rLiUwObMTRM948J04NoYBx1jwMZLNEFs6os6\r\n"\
"91J8B5oHlgQAsgcXjoXORxncgt2d/fZyr9NjVevc/GIkcbfVAAWohFCQmjpgLESW\r\n"\
"iTPaRlLyeLLjTb+WrSOKosinVH+1R8ozfBnkzO+tf9lpBzdP9QCDYzNQeU4Xp0wJ\r\n"\
"8Neo5tA4tioDEZjkbsHL+bVvssqOwfupR9b03Z2ZLfgEzs3TwYbPB7ULJQrdqe9x\r\n"\
"QtpaFSsDeyKJ6CR8yEHz5d0lypPeRLCjZGb49Yo3FKhMdzP+ZMT+Ku001cr4GwID\r\n"\
"AQABo1AwTjAdBgNVHQ4EFgQU4FfBsWrgpoGtvuF+3XN9kbUx0yUwHwYDVR0jBBgw\r\n"\
"FoAU4FfBsWrgpoGtvuF+3XN9kbUx0yUwDAYDVR0TBAUwAwEB/zANBgkqhkiG9w0B\r\n"\
"AQsFAAOCAYEAqfVX0LhhxYZ/KsC9jhWYHgmYggVcsTa7AtZsNKW79TLmz98iWdfM\r\n"\
"wpi44rwbn1Xek5pDH/rimaJuwxcX8Q4cFHqeQJStRkcuU6CgyurVBpjGWRv5qt3W\r\n"\
"nJc/z92x3TPzW1VJv5rXj4pzX9N4hlAHGGzFBm+VMAexS006/dksGa9uEKE/2A0+\r\n"\
"9W/V9YbcjhejdENiRAvJB4J0QsOrNUjsH5bPEa3CxdXbKOQzGjJtS7f0BH38Fmyi\r\n"\
"C+Cui8U0c+BwRGY3HXL7ANhCe0vdUbUGCG2L6byRvf1TIkuGpi0RxtQfEF3sTDH5\r\n"\
"jAot50rJhbckQyLH0xkIOQ9qmU/gbt/wgoZ9AzUVIyh0RsyWo19BGz2DpsuYNBXD\r\n"\
"4jqL4NMqsyGRq5YUTrJlIi9PVUp176Ec79xSffvUbitiq9fMmxuhsRbkP4piM1TE\r\n"\
"D5oXKPme86RvR1/foRqAdbJg5RPYdah3LdOlAE2HePVy6b0xQ5dcCHaqHmR2SVlY\r\n"\
"m7TQs6tfvfhy\r\n"\
"-----END CERTIFICATE-----\r\n"




////my user for mqtt
#define GLOBALSIGN_USER   \
"-----BEGIN CERTIFICATE-----\r\n"\
"MIIEjzCCAvcCCQD57PGIsuol7jANBgkqhkiG9w0BAQsFADCBhzELMAkGA1UEBhMC\r\n"\
"Q04xCzAJBgNVBAgMAkFIMQswCQYDVQQHDAJIRjEcMBoGA1UECgwTRGVmYXVsdCBD\r\n"\
"b21wYW55IEx0ZDEYMBYGA1UEAwwPMjIwLjE4MC4yMzkuMjEyMSYwJAYJKoZIhvcN\r\n"\
"AQkBFhdlZGRpZS56aGFuZ0BxdWVjdGVsLmNvbTAgFw0yMTAxMjcwMzQyMjZaGA8y\r\n"\
"MTIxMDEwMzAzNDIyNlowgYgxCzAJBgNVBAYTAkNOMQswCQYDVQQIDAJBSDELMAkG\r\n"\
"A1UEBwwCSEYxHDAaBgNVBAoME0RlZmF1bHQgQ29tcGFueSBMdGQxGDAWBgNVBAMM\r\n"\
"DzIyMC4xODAuMjM5LjIxMjEnMCUGCSqGSIb3DQEJARYYZWRkaWUuemhhbmdAcWkI\r\n"\
"ZWN0ZWwuY29tMIIBojANBgkqhkiG9w0BAQEFAAOCAY8AMIIBigKCAYEAstvaVIF1\r\n"\
"JDm0/ABzFUvcCSDY7El0W5cmBRz1i+W0G+GL3iPjUvdNpWdNC3KovCXVUcFpA5Jh\r\n"\
"EPp3rYMEELHAVh1Ylbk4kdGh6I3SRGx+qaISQmtjRiwhZu1l6HavOhPWfY+pWc6F\r\n"\
"uIqoyYvuB01SJIIeqxz/1A3rPrM4GgARPYDnSBhxBxfY8lYjKbT6kgxnlkZOMyMb\r\n"\
"HZYzcbJLUdeH0457cMFQenvSH3roAMrUKQEGG4iiHH6ozaN7IyNat91fM9f3uzpl\r\n"\
"h6T8UEB0SC45uVzQNB0RY4Yof8VNfjkN8djTutzkB/1Fhz2y4eunqlVK40J0MgCR\r\n"\
"sD7MCjSDEgl4mHe1XEH26AggszyE0qfIUhbyYIT3h1/4Gor3YbKKQsl6GpJ/2+fU\r\n"\
"bOkhiAbtGvefE3DvFZzcsu//rWT7ZO0F0WOj6VPcfNDZP5kQVxnfbqXRS6YTbWcG\r\n"\
"6SBmIL05/DXEDRGEgxov2WqbdNO/0hL/xO0HFyEs6ELWFUT5wpd2Yx3XAgMBAAEw\r\n"\
"DQYJKoZIhvcNAQELBQADggGBAH8yycWHdMKyISll4tz9nKCaXTwZDB87FhRIjnzu\r\n"\
"Lg1wgK08L/owI696qY3Z8fhMjJvnxf1X3Oj8dgddbaH1quVDYFt7lKulMEs2t0/F\r\n"\
"lruTkclXOYRi4G7OQ5DSjI9cVF69zxsZ1mH9hWnvhSbKl0Du1w/ZUaOJeoNaV8vv\r\n"\
"EG2marvmV07VOhwdbEJbvtGN14kOGHJq2SzeMXoC9MBHWdQpQu0SHl0bBs4paENd\r\n"\
"Cl7R3Mz6RDuIhd4U59Hsj3sEPlSHbyldbWRUyAgFhf1Ja3AGsEPpX3qiRO6EllpP\r\n"\
"1pgMqUyyXjNScYlYKuamZqa+KkMo0uLYKt7e4Q/oiWNMtB/LNeUOOpiE33l0VKc6\r\n"\
"zDk+DXs7FZ/U5YOqahS0P/Ob8nfnJVNKtQebrOFXxCn+Agps+5882KWpAp7pGfwD\r\n"\
"jZpeYEKOWgYl3UQMd2c5SbICqOMocUWDTSknUPy9odWKGKCKpZ56/qD2BS45ShmE\r\n"\
"Jyhxprn0pX/Kz8jW2l1Wk2LEqg==\r\n"\
"-----END CERTIFICATE-----\r\n"




////my pkey for mqtt
#define GLOBALSIGN_PKEY   \
"-----BEGIN RSA PRIVATE KEY-----\r\n"\
"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\r\n"\
"-----END RSA PRIVATE KEY-----\r\n"


const char mbedtls_root_certificate[] = 
	GLOBALSIGN_ROOT_CA
;

const char mbedtls_user_certificate[] = 
	GLOBALSIGN_USER
;

const char mbedtls_pkey_certificate[] = 
	GLOBALSIGN_PKEY
;

#endif




const size_t mbedtls_root_certificate_len = sizeof(mbedtls_root_certificate);
const size_t mbedtls_user_certificate_len = sizeof(mbedtls_user_certificate);
const size_t mbedtls_pkey_certificate_len = sizeof(mbedtls_pkey_certificate);
#if 0
///client client.crt
#define GLOBALSIGN_Client_crt   \
"-----BEGIN CERTIFICATE-----\r\n"  \
"MIIDkDCCAngCCQCb4RI7B1uRIDANBgkqhkiG9w0BAQsFADCBhTELMAkGA1UEBhMC\r\n"  \
"Q04xEDAOBgNVBAgMB3NoYWdoYWkxETAPBgNVBAcMCHNoYW5naGFpMQ4wDAYDVQQK\r\n"  \
"DAVCRUtFTjEQMA4GA1UECwwHd2lmaS1nZTESMBAGA1UEAwwJd2lmaS10ZWFtMRsw\r\n"  \
"GQYJKoZIhvcNAQkBFgxjd2xpbjE2My5jb20wHhcNMjAwODExMDU1MzI1WhcNMzAw\r\n"  \
"ODA5MDU1MzI1WjCBjTELMAkGA1UEBhMCQ04xETAPBgNVBAgMCHNoYW5naGFpMREw\r\n"  \
"DwYDVQQHDAhzaGFuZ2hhaTEOMAwGA1UECgwFYmVrZW4xEzARBgNVBAsMCmJla2Vu\r\n"  \
"LXdpZmkxFjAUBgNVBAMMDTE5Mi4xNjguMTkuMjQxGzAZBgkqhkiG9w0BCQEWDGN3\r\n"  \
"bGluMTYzLmNvbTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAMSKa+Cn\r\n"  \
"9wVL7H1/zY02jJTHmq3Y6XfMwwkHsCE3Oag62snjNs60LQCYIQvGMXRmz9hIuWRW\r\n"  \
"hAA627TLIAvGhXG5mFkuG3HJJLQLdTGuWobjTL74/hrSAstmJM/eg9M1FHvWO9rq\r\n"  \
"bTNagybd9k2BjdZdah1QFlDagECtAbBKq89riKqvlwaIJP43Xb0dYXZ223iHsNDv\r\n"  \
"wYO9TU9Cl7U49UURUA7hw649cPtVA++uKQJSsqEpHF1/Q0fJcCnFqfPSZsqdhBmO\r\n"  \
"p4S1t+xqpautEiGWyBcdOZ1Og4a5gio4hBt4hDpLWXIF/mrjGMvImm4RlOkAhbil\r\n"  \
"6nZz+ZwXl2RD4ZkCAwEAATANBgkqhkiG9w0BAQsFAAOCAQEAkY7PGZkxn4zoV+/a\r\n"  \
"EBcZ06V9PqCYtn+8/r4ciBW1GHjveOXX5plVkaKpkx1QFOS5RV96vk4N10HnKO9V\r\n"  \
"YjRqPN3maiQ+bn3Yve0zopMPbNiA+oTZqrP8jRlpTujCo7XE6fKXfQYpRXijXDHB\r\n"  \
"Ty4pURq8UkEmyZyFoSUYgib2aG0K656OOG5LSRRda4NSWJilhLB5Aheb7gfVcLut\r\n"  \
"eUq28qFSx0BC+Yf2IaMMnOIO1HD8mgdEgktGlX7eapAive3g/sFmaX9TPzV6hJ5U\r\n"  \
"s4OvpG2D3BzwJH5pmoi1ovOi2m0WauR8Z3W3m7kywLEUtfEbX0W+Q02CPMYw1jmi\r\n"  \
"D/qgUw==\r\n"  \
"-----END CERTIFICATE-----\r\n"


const char mbedtls_client_certificate[] =
	GLOBALSIGN_ROOT_CA
	GLOBALSIGN_Client_crt
;
const size_t mbedtls_client_certificate_len = sizeof(mbedtls_client_certificate);
#endif



