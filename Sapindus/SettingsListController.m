//
//  SettingsListController.m
//  monkcGame
//
//  Created by Sun YuLi on 16/4/23.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#import "SettingsListController.h"

@implementation SettingsListController

- (instancetype)initWithCoder:(NSCoder *)coder
{
    self = [super initWithCoder:coder];
    if (self) {
        //Skybox
        self.skyboxSwitch = [UISwitch new];
        self.skyboxSwitch.on = [[NSUserDefaults standardUserDefaults] boolForKey:@"SkyboxOn"];
        [self.skyboxSwitch addTarget:self
                              action:@selector(onSkyboxSwitchValueChanged:)
                    forControlEvents:UIControlEventValueChanged];
        //Skysph
        self.skysphSwitch = [UISwitch new];
        self.skysphSwitch.on = [[NSUserDefaults standardUserDefaults] boolForKey:@"SkysphOn"];
        [self.skysphSwitch addTarget:self
                              action:@selector(onSkysphSwitchValueChanged:)
                    forControlEvents:UIControlEventValueChanged];
        //MAA
        self.maaSwitch = [UISwitch new];
        self.maaSwitch.on = [[NSUserDefaults standardUserDefaults] boolForKey:@"MAAOn"];
        [self.maaSwitch addTarget:self
                           action:@selector(onMAASwitchValueChanged:)
                 forControlEvents:UIControlEventValueChanged];
    }
    return self;
}

-(void)viewDidLoad
{
    [super viewDidLoad];
    
    self.tableView.dataSource = self;
    self.tableView.delegate = self;
    
    self.listData = [NSMutableArray arrayWithObjects:
                     @"Tutorial",
                     @"3rd Party Library License",
                     @"3rd Party Demo Contents License",
                     @"Skybox On",
                     @"SkySphere On",
                     @"Enable MAA (multisampling anti-alias)",
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
        if (indexPath.row == 3) {
            cell.accessoryView = self.skyboxSwitch;
        }
        if (indexPath.row == 4) {
            cell.accessoryView = self.skysphSwitch;
        }
        if (indexPath.row == 5) {
            cell.accessoryView = self.maaSwitch;
        }
    }
    return cell;
}

- (void) onSkyboxSwitchValueChanged:(id)sender
{
    UISwitch* sw = (UISwitch*)sender;
    if (sw.isOn) {
        [[NSUserDefaults standardUserDefaults] setBool:YES forKey:@"SkyboxOn"];
    }else{
        [[NSUserDefaults standardUserDefaults] setBool:NO forKey:@"SkyboxOn"];
    }
}

- (void) onSkysphSwitchValueChanged:(id)sender
{
    UISwitch* sw = (UISwitch*)sender;
    if (sw.isOn) {
        [[NSUserDefaults standardUserDefaults] setBool:YES forKey:@"SkysphOn"];
    }else{
        [[NSUserDefaults standardUserDefaults] setBool:NO forKey:@"SkysphOn"];
    }
}

- (void) onMAASwitchValueChanged:(id)sender
{
    UISwitch* sw = (UISwitch*)sender;
    if (sw.isOn) {
        
        [[NSUserDefaults standardUserDefaults] setBool:YES forKey:@"MAAOn"];
    }else{
        [[NSUserDefaults standardUserDefaults] setBool:NO forKey:@"MAAOn"];
    }
}

@end
