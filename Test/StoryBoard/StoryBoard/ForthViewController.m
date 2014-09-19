//
//  ForthViewController.m
//  StoryBoard
//
//  Created by gavingu on 14-6-18.
//  Copyright (c) 2014年 sundayliu. All rights reserved.
//

#import "ForthViewController.h"

@interface ForthViewController ()

@end

@implementation ForthViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view.
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (IBAction)backBtnClicked:(id)sender
{
    [self dismissModalViewControllerAnimated:TRUE];
}

@end
