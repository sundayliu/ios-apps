//
//  ProcessScanner.cpp
//  HelloWorld
//
//  Created by gavingu on 14-6-19.
//  Copyright (c) 2014年 sundayliu. All rights reserved.
//

#include "ProcessScanner.h"

#include <stdlib.h>
#include <sys/sysctl.h>
#include <string.h>

#include <mach/vm_map.h>
#include <mach/mach.h>
#include <mach-o/loader.h>
#include <mach-o/nlist.h>
#include <mach-o/dyld_images.h>

#include <dlfcn.h>

#include <stdio.h>

int getProcessList(char processList[][17],unsigned int len)
{
    int ret = 0;
    int mib[4] = {CTL_KERN,KERN_PROC,KERN_PROC_ALL,0};
    size_t miblen = sizeof(mib)/sizeof(mib[0]);
    size_t size = 0;
    int status = sysctl(mib,miblen,NULL,&size,NULL,0);
    struct kinfo_proc* process = (struct kinfo_proc*)malloc(size);
    if (process != NULL)
    {
        status = sysctl(mib, miblen, process, &size, NULL, 0);
        if (status == 0)
        {
            int numberOfProcess = size / sizeof(struct kinfo_proc);
            int i = 0;
            for (i = 0; i < numberOfProcess && i < len; i++)
            {
                //char processName[17] = {0};
                strncpy(processList[i], process[i].kp_proc.p_comm,16);
            }
            
            ret = i;
        }
        
        free(process);
    }
    
    return ret;
}

bool isMachOHeader(vm_address_t address, mach_msg_type_number_t size)
{
    bool result = false;
    if (size >= 4096)
    {
        vm_offset_t data = 0;
        mach_msg_type_number_t dataLen = 0;
        kern_return_t kr = KERN_SUCCESS;
        kr = vm_read(mach_task_self(),
                     address,
                     4096,
                     &data,
                     &dataLen);
        if (kr == KERN_SUCCESS)
        {
            int magicNumber = 0xfeedface;
            unsigned char* p = (unsigned char*)data;
            if (p && memcmp(p+1, ((unsigned char*)&magicNumber) + 1, 3) == 0)
            {
                result = true;
            }
            vm_deallocate(mach_task_self(), data, dataLen);
        }
    }
    return result;
}

bool isSameRegionAttr(const vm_region_basic_info_data_t* a, const vm_region_basic_info_data_t* b)
{
    bool ret = true;
    if (a == NULL)
    {
        if (b == NULL)
        {
            ret = true;
        }
        else
        {
            ret = false;
        }
    }
    else
    {
        if (b == NULL)
        {
            ret = false;
        }
        else
        {
            ret = (a->protection == b->protection
                   && a->max_protection == b->max_protection
                   && a->inheritance == b->inheritance
                   && a->reserved == b->reserved
                   && a->shared == b->shared);
        }
    }
    return ret;
}

void* getImageSymbols(struct mach_header* mh, const char* symbol)
{
    void* result = NULL;
    
    struct load_command* lc = (struct load_command*)((char*)mh + sizeof(struct mach_header));
    struct segment_command* segLinkEdit = NULL;
    struct segment_command* segData = NULL;
    struct segment_command* segText = NULL;
    struct symtab_command* lcSymtab = NULL;
    
    for (int i = 0; i < mh->ncmds; i++)
    {
        if (lc->cmd == LC_SEGMENT)
        {
            struct segment_command* seg = (struct segment_command*)lc;
            if (strcmp(seg->segname, "__LINKEDIT") == 0)
            {
                segLinkEdit = seg;
            }
            else if (strcmp(seg->segname, "__TEXT") == 0)
            {
                segText = seg;
            }
            else if (strcmp(seg->segname,"__DATA") == 0)
            {
                segData = seg;
            }
            
            if (segLinkEdit && segText && segData && lcSymtab)
            {
                break;
            }
        }
        else if (lc->cmd == LC_SYMTAB)
        {
            lcSymtab = (symtab_command*)lc;
            if (segText && segData && segLinkEdit && lcSymtab)
            {
                break;
            }
        }
        lc = (struct load_command*)((char*)lc + lc->cmdsize);
    }
    
    if (segText && segData && segLinkEdit && lcSymtab && lcSymtab->symoff && lcSymtab->stroff)
    {
        unsigned int delta = (segText->vmsize - segText->filesize) + (segData->vmsize - segData->filesize);
        struct nlist  *symtab = (struct nlist*)((unsigned long)mh + lcSymtab->symoff + delta);
        const char* strtab = (const char*)((unsigned long)mh + lcSymtab->stroff + delta);
        for (int i = 0; i < lcSymtab->nsyms; i++)
        {
            struct nlist &sym = symtab[i];
            if (sym.n_sect == NO_SECT) continue;
            char* symName = (char*)(strtab + symtab[i].n_un.n_strx);
            if (strcmp(symName, symbol) == 0)
            {
                unsigned long dyld_image_info_offset = sym.n_value - segText->vmaddr;
                result = (void*)((unsigned long)mh + dyld_image_info_offset);
                break;
            }
        }
    }
    return result;
}

void parseImageInfos(void* imageInfo,pfnTraversalVMCallback callback, void* param)
{
    //mach_msg_type_number_t dataCount = 0;
    
    //dataCount = sizeof(dyld_all_image_infos);
    struct dyld_all_image_infos* dyldAllImageInfos = (struct dyld_all_image_infos*)imageInfo;
    int imageCount = 0;
    imageCount = dyldAllImageInfos->infoArrayCount;
    struct dyld_image_info* dyldImageInfo = (dyld_image_info*)(dyldAllImageInfos->infoArray);
    for (int i = 0; i < imageCount; i++)
    {
        const char* name = dyldImageInfo[i].imageFilePath;
        intptr_t baseAddress = (intptr_t)dyldImageInfo[i].imageLoadAddress;
        if (name == NULL)
        {
            // main module
            struct dl_info mainDLInfo = {0};
            dladdr((const void*)baseAddress,&mainDLInfo);
            if (mainDLInfo.dli_fname != NULL)
            {
                baseAddress = (intptr_t)mainDLInfo.dli_fbase;
                name = mainDLInfo.dli_fname;
            }
            
        }
        
        printf("Addr:%08lX Name:%s\n",baseAddress,name);
    }
    
}

int processVM(vm_address_t address, mach_msg_type_number_t size, pfnTraversalVMCallback callback,void* param)
{
    int result = 0;
    if ( size >= 4096)
    {
        if (isMachOHeader(address, size))
        {
            struct load_command* lc = NULL;
            struct dylinker_command* dyld = NULL;
            struct mach_header* mh = NULL;
            char* dyld_name = NULL;
            int flag = 0;
            
            mh = (struct mach_header*)(unsigned char*)address;
            lc = (load_command*)((char*)mh + sizeof(struct mach_header));
            for (int i = 0; i < mh->ncmds; i++)
            {
                if (lc->cmd == LC_ID_DYLINKER)
                {
                    dyld = (struct dylinker_command*)lc;
                    dyld_name = (char*)dyld + dyld->name.offset;
                    flag = 1;
                    break;
                }
                lc = (load_command*)((unsigned char*)lc + lc->cmdsize);
            }
            
            if (flag == 1)
            {
                if (strstr(dyld_name,"/lib/dyld") != NULL)
                {
                //void* symbolPointer = NULL;
                void* handle = dlopen("/usr/lib/dyld", RTLD_NOW);
                if (handle != NULL)
                {
                    dlclose(handle);
                }
                void* symbolPointer = getImageSymbols((struct mach_header*)address, "_dyld_all_image_infos");
                if (symbolPointer != NULL)
                {
                    parseImageInfos(symbolPointer, callback, param);
                }
                }
            }
        }
    }
    return result;
}

void traversalVMRegion(pfnTraversalVMCallback callback,void*param)
{
    kern_return_t kr = KERN_SUCCESS;
    vm_size_t vmsize = 0;
    vm_region_basic_info_data_t info;
    vm_address_t address = 0;
    
    while (kr != KERN_INVALID_ADDRESS)
    {
        mach_msg_type_number_t count = VM_REGION_BASIC_INFO_COUNT;
        memory_object_name_t object;
        
        kr = vm_region(mach_task_self(),
                       &address,
                       &vmsize,
                       VM_REGION_BASIC_INFO,
                       (vm_region_info_t)&info,
                       &count,
                       &object);
        if (kr == KERN_SUCCESS)
        {
            printf("address:0x%08X,size:0x%08X\n",address,vmsize);
            //memcpy(&prevInfo,&info,sizeof(vm_region_basic_info_data_t));
            if (processVM(address,vmsize,callback,param) == 1)
            {
                break;
            }
            address += vmsize;
        }
        
//        if (address > 0x40000000)
//        {
//            break;
//        }

    }
}

int getProcessModuleList(char moduleList[][0x100], unsigned int len)
{
    int ret = 0;
    traversalVMRegion(NULL,NULL);
    return ret;
}