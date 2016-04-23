//
//  SettingsListController.m
//  monkcGame
//
//  Created by Sun YuLi on 16/4/23.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#import "SettingsListController.h"

@implementation SettingsListController

-(void)viewDidLoad
{
    self.tableView.dataSource = self;
    self.tableView.delegate = self;
    
    self.listData = [NSMutableArray arrayWithObjects:
                     @"Backgroud Color",
                     @"Rotation Direction",
                     @"Show FPS", nil];
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return self.listData.count;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    UITableViewCell* cell = [tableView dequeueReusableCellWithIdentifier:@"SettingsListCell"];
    NSInteger i = indexPath.row;
    if (cell && i <= self.listData.count) {
        cell.textLabel.text = [self.listData objectAtIndex:i];
    }
    return cell;
}

@end
