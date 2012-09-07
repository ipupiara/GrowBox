// GrowBoxSoapLines.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


#include "../commonSoap/soapH.h" // obtain the generated stub 
#include "../commonSoap/states.nsmap"


int main(int argc, char* argv[])
{



	GrowBoxInfo result;

   struct soap soap; 
   soap_init(&soap); // initialize runtime environment (only once) 
									// url, action ??
   if (soap_call_ns__getGrowBoxInfo(&soap, "http://127.0.0.1:18083/soap", NULL, &result) == SOAP_OK) {
			printf("Hygro Cnt   = %i <br> \n",result.HygroCnt);
			printf("Temperature = %.1f <br>\n",result.Temperature); 
			printf("Humidity    = %.1f <br>\n",result.Humidity);
			printf("State       = %s  <br> \n",result.State);
   }
   else {
      soap_print_fault(&soap, stderr);
	  printf("GrowBoxSoapLines Error \n");
	}
	
   


   soap_destroy(&soap); // delete deserialized class instances (for C++ only) 
   soap_end(&soap); // remove deserialized data and clean up 
   soap_done(&soap); // detach the gSOAP environment
//	TRACE0("end  CClientDlg::OnTimer \n");


	return 0;
}

