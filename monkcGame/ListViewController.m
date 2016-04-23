//
//  ListViewController.m
//  monkcGame
//
//  Created by Sun YuLi on 16/4/20.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#import "ListViewController.h"
#import "MC3DiOS.h"

@implementation ListViewController {
    
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
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    NSInteger i = indexPath.row;
    NSString* filename = [self.listData objectAtIndex:i];
    if (filename && ![filename isEqualToString:@""]) {
        NSString* file = [filename stringByDeletingPathExtension];
        [self performSelector:@selector(delayOpen:) withObject:file afterDelay:0.2f];
    }
}

-(void) delayOpen:(NSString*)name
{
    const char* cfile = [name cStringUsingEncoding:NSUTF8StringEncoding];
    static int lock = 0;
    if (lock == 0) {
        onOpenFile(cfile, &lock);
        [[NSNotificationCenter defaultCenter] postNotificationName:@"sapindus.open.model" object:name];
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

@end
