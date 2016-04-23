//
//  ListViewController.m
//  monkcGame
//
//  Created by Sun YuLi on 16/4/20.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#import "ListViewController.h"

@implementation ListViewController

- (instancetype)init
{
    self = [super init];
    if (self) {
        
    }
    return self;
}

//UITableViewDelegate
- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    [self dismissViewControllerAnimated:YES completion:NULL];
}

-(IBAction)unwindToList:(UIStoryboardSegue *)segue
{

}

@end
