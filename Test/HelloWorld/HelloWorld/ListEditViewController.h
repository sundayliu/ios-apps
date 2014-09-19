//
//  ListEditViewController.h
//  HelloWorld
//
//  Created by gavingu on 14-6-18.
//  Copyright (c) 2014年 sundayliu. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface ListEditViewController : UIViewController

@property(strong,nonatomic)IBOutlet UITextField* editText;
@property(copy,nonatomic)NSDictionary* selection;
@property(weak,nonatomic)id preViewController;


@end
