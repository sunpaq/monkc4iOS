//
//  MasterNavController.m
//  monkcGame
//
//  Created by Sun YuLi on 16/4/21.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#import "MasterNavController.h"

@implementation MasterNavController

-(void)viewDidLoad
{
    [super viewDidLoad];
    
    
}

-(IBAction) onCloseClicked:(id)sender
{
    [sender dismissViewControllerAnimated:YES completion:nil];
}

@end
