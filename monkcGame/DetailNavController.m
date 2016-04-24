//
//  DetailNavController.m
//  monkcGame
//
//  Created by Sun YuLi on 16/4/23.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#import "DetailNavController.h"

@implementation DetailNavController

-(void)viewDidLoad
{
    [super viewDidLoad];
    
    //0.05, 0.25, 0.35, 1.0
    self.navigationBar.barTintColor = [UIColor colorWithRed:0.05 green:0.25 blue:0.35 alpha:0.6];
    self.navigationBar.tintColor = [UIColor colorWithRed:1.0 green:1.0 blue:1.0 alpha:1.0];
    
//    self.toolbar.barTintColor = [UIColor colorWithRed:0.05 green:0.25 blue:0.35 alpha:0.6];
//    self.toolbar.tintColor = [UIColor colorWithRed:1.0 green:1.0 blue:1.0 alpha:1.0];
}

@end
