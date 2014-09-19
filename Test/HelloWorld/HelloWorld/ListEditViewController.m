//
//  ListEditViewController.m
//  HelloWorld
//
//  Created by gavingu on 14-6-18.
//  Copyright (c) 2014年 sundayliu. All rights reserved.
//

#import "ListEditViewController.h"



@interface ListEditViewController ()

@end

@implementation ListEditViewController

@synthesize preViewController;
@synthesize selection;

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
    _editText.text = [selection objectForKey:@"object"];
    [_editText becomeFirstResponder];
}

- (void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
    
    [_editText endEditing:YES];
    NSIndexPath* indexPath = [selection objectForKey:@"indexPath"];
    id object = _editText.text;
    NSDictionary* editedSelection = [NSDictionary dictionaryWithObjectsAndKeys:
                                     indexPath,@"indexPath",
                                     object,@"object",
                                     nil];
    [preViewController setValue:editedSelection forKey:@"editedSelection"];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
