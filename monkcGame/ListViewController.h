//
//  ListViewController.h
//  monkcGame
//
//  Created by Sun YuLi on 16/4/20.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface ListViewController : UITableViewController <UITableViewDelegate>
@property IBOutlet UITableViewController* detailViewController;
-(IBAction) dismissSelf;
@end
