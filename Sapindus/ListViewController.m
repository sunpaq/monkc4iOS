//
//  ListViewController.m
//  monkcGame
//
//  Created by Sun YuLi on 16/4/20.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#import "ListViewController.h"
#import "DetailNavController.h"
#import "GameViewController.h"
#import "MC3DiOS.h"

@implementation ListViewController {
    DetailNavController* _dnc;
    GameViewController*  _gvc;
}

-(void)viewDidLoad
{
    [super viewDidLoad];
        
    self.tableView.dataSource = self;
    self.tableView.delegate = self;
    self.listData = [NSMutableArray array];
                     
    NSArray<NSURL*>* paths =[[NSBundle mainBundle] URLsForResourcesWithExtension:@"obj" subdirectory:nil];
    for (NSURL* path in paths) {
        NSString* name = [path lastPathComponent];
        [self.listData addObject:name];
    }
    
    [self.listData addObject:@"TESTCUBE"];
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    NSInteger i = indexPath.row;
    NSString* filename = [self.listData objectAtIndex:i];
    if (filename && ![filename isEqualToString:@""]) {
        
        [_gvc setFilename:[filename stringByDeletingPathExtension]];
        
    }
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return self.listData.count;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    UITableViewCell* cell = [tableView dequeueReusableCellWithIdentifier:@"FilesListCell"];
    NSInteger i = indexPath.row;
    if (cell && i <= self.listData.count) {
        cell.textLabel.text = [self.listData objectAtIndex:i];
    }
    return cell;
}

//unwind segue
-(IBAction)unwindToList:(UIStoryboardSegue *)segue
{
    //we can save data here
    
}

-(void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
{
    _dnc = (DetailNavController*)segue.destinationViewController;
    _gvc = _dnc.childViewControllers.firstObject;
}

@end
