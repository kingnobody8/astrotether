#include "resource_path.h"
#import <Foundation/Foundation.h>

std::string getResourcePath(void)
{
    NSBundle* mainBundle = [NSBundle mainBundle];
    
    if (mainBundle != nil)
    {
        NSString* path = [mainBundle resourcePath];
        return [path UTF8String] + std::string("/");
    }
	
	return "";
}