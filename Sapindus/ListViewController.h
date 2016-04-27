//
//  ListViewController.h
//  monkcGame
//
//  Created by Sun YuLi on 16/4/20.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface ListViewController : UITableViewController

@property (nonatomic) NSMutableArray* listData;

-(IBAction)unwindToList:(UIStoryboardSegue *)segue;

@end
