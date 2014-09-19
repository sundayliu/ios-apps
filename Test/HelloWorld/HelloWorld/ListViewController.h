//
//  ListViewController.h
//  HelloWorld
//
//  Created by gavingu on 14-6-18.
//  Copyright (c) 2014年 sundayliu. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface ListViewController : UITableViewController

@property(strong,nonatomic) NSMutableArray* listArray;
@property(copy,nonatomic) NSDictionary* editedSelection;

@end
