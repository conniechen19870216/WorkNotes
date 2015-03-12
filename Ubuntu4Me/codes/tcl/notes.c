/*
 * =====================================================================================
 *
 *       Filename:  notes.c
 *
 *    Description:  call tcl scripts
 *
 *        Version:  1.0
 *        Created:  2014年03月26日 13时16分09秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Kavon Ma (mn), kavon.ma@alcatel-lucent.com
 *        Company:  Alcatel-Lucent
 *
 * =====================================================================================
 */

#include <tcl.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
	Tcl_Interp *pTclInterp = Tcl_CreateInterp();
	if (!pTclInterp)
	{
		printf("Tcl Interpreter could not be created\n");
		return 0;
	}

	if (Tcl_EvalFile(pTclInterp, "notes.tcl") == TCL_ERROR)
	{
		printf("execute the tcl script failed\n");
		return 0;
	}

	Tcl_DeleteInterp(pTclInterp);
	return 0;
}
