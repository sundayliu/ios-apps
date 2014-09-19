//
//  DateAndTimeViewController.m
//  HelloWorld
//
//  Created by gavingu on 14-6-18.
//  Copyright (c) 2014年 sundayliu. All rights reserved.
//

#import "DateAndTimeViewController.h"

@interface DateAndTimeViewController ()

@end

@implementation DateAndTimeViewController

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

- (void) viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    NSDate* now = [NSDate date];
    _dateLabel.text = [NSDateFormatter
                      localizedStringFromDate:now
                      dateStyle:NSDateFormatterLongStyle
                      timeStyle:NSDateFormatterNoStyle];
    _timeLabel.text = [NSDateFormatter
                       localizedStringFromDate:now
                       dateStyle:NSDateFormatterNoStyle
                       timeStyle:NSDateFormatterLongStyle];
    
    
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
