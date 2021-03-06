//
//  AppDelegate.m
//  monkcGame
//
//  Created by SunYuLi on 5/15/15.
//  Copyright (c) 2015 oreisoft. All rights reserved.
//

#import "AppDelegate.h"
#import "MC3DiOS.h"

int getSkyboxOn()
{
    if ([[NSUserDefaults standardUserDefaults] boolForKey:@"SkyboxOn"]) {
        return 1;
    }
    if ([[NSUserDefaults standardUserDefaults] boolForKey:@"SkysphOn"]) {
        return 2;
    }
    return 0;
}

@interface AppDelegate ()

@end

@implementation AppDelegate

-(void) delayOpen:(NSString*)name
{
    const char* cfile = [name cStringUsingEncoding:NSUTF8StringEncoding];
    onOpenFile(cfile);
    [[NSNotificationCenter defaultCenter] postNotificationName:@"sapindus.open.model.stop" object:name];
}

- (void)openExtraModel:(NSString*)filename
{
    if (filename && ![filename isEqualToString:@""]) {
        NSString* file = [filename stringByDeletingPathExtension];
        [[NSNotificationCenter defaultCenter] postNotificationName:@"sapindus.open.model.start" object:file];
        [self performSelector:@selector(delayOpen:) withObject:file afterDelay:0.2f];
    }
}

- (BOOL)application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(NSString *)sourceApplication annotation:(id)annotation {
    GLchar* obj3d = (GLchar*)[[url path] UTF8String];
    onOpenExternalFile(obj3d);
    return YES;
}

- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application
{
    onReceiveMemoryWarning();
}

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    // Override point for customization after application launch.
    onAppStart();
    return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application {
    // Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
    // Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
}

- (void)applicationDidEnterBackground:(UIApplication *)application {
    // Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later.
    // If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
}

- (void)applicationWillEnterForeground:(UIApplication *)application {
    // Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
    // Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
}

- (void)applicationWillTerminate:(UIApplication *)application {
    // Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
}

@end
