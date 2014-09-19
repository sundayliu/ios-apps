//
//  ProcessScanner.h
//  HelloWorld
//
//  Created by gavingu on 14-6-19.
//  Copyright (c) 2014年 sundayliu. All rights reserved.
//

#ifndef HelloWorld_ProcessScanner_h
#define HelloWorld_ProcessScanner_h

#if defined(__LP64__)||defined(__64BIT__)||defined(_LP64)||(_WORDSIZE==64)
#endif

#ifdef __cplusplus
extern "C"{
#endif
    
    struct module_info_t
    {
        char* name;
        unsigned int baseAddress;
    };
typedef int(* pfnTraversalVMCallback)(const struct module_info_t* info, void* param);

int getProcessList(char processList[][17],unsigned int len);
    
int getProcessModuleList(char moduleList[][0x100],unsigned int len);
    
int getModuleBase(const char* moduleName);
    
#ifdef __cplusplus
}
#endif

#endif
