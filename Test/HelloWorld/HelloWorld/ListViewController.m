//
//  ListViewController.m
//  HelloWorld
//
//  Created by gavingu on 14-6-18.
//  Copyright (c) 2014年 sundayliu. All rights reserved.
//

#import "ListViewController.h"

@interface ListViewController ()

@end

@implementation ListViewController

@synthesize listArray;
@synthesize editedSelection;

- (id)initWithStyle:(UITableViewStyle)style
{
    self = [super initWithStyle:style];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];

    // Uncomment the following line to preserve selection between presentations.
    // self.clearsSelectionOnViewWillAppear = NO;
 
    // Uncomment the following line to display an Edit button in the navigation bar for this view controller.
    // self.navigationItem.rightBarButtonItem = self.editButtonItem;
    
    listArray = [[NSMutableArray alloc] initWithObjects:@"Dog",@"Pig",@"Cat",@"Book", nil];
    
}

- (void)setEditedSelection:(NSDictionary *)dict
{
    if (![dict isEqual:editedSelection])
    {
        editedSelection = dict;
        NSIndexPath* indexPath = [dict objectForKey:@"indexPath"];
        id newValue = [dict objectForKey:@"object"];
        [listArray replaceObjectAtIndex:indexPath.row withObject:newValue];
        [self.tableView reloadRowsAtIndexPaths:[NSArray arrayWithObject:indexPath]
                              withRowAnimation:UITableViewRowAnimationAutomatic];
    }
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

#pragma mark - Table view data source

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
//#warning Potentially incomplete method implementation.
    // Return the numb
    return 1;
    //return 0;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
//#warning Incomplete method implementation.
    // Return the number of rows in the section.
    //return 0;
    return [listArray count];
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    //static NSString *CellIdentifier = @"Cell";
    //UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier forIndexPath:indexPath];
    
    // Configure the cell...
    NSUInteger row = [indexPath row];
    NSString* identifier = nil;
    if (row % 2 == 0)
    {
        identifier = @"GreenIdentifier";
    }
    else
    {
        identifier = @"RedIdentifier";
    }
    
    UITableViewCell* cell = [tableView dequeueReusableCellWithIdentifier:identifier];
    UILabel* cellLabel = (UILabel*)[cell viewWithTag:1];
    cellLabel.text = [listArray objectAtIndex:row];
    return cell;
}

/*
// Override to support conditional editing of the table view.
- (BOOL)tableView:(UITableView *)tableView canEditRowAtIndexPath:(NSIndexPath *)indexPath
{
    // Return NO if you do not want the specified item to be editable.
    return YES;
}
*/

/*
// Override to support editing the table view.
- (void)tableView:(UITableView *)tableView commitEditingStyle:(UITableViewCellEditingStyle)editingStyle forRowAtIndexPath:(NSIndexPath *)indexPath
{
    if (editingStyle == UITableViewCellEditingStyleDelete) {
        // Delete the row from the data source
        [tableView deleteRowsAtIndexPaths:@[indexPath] withRowAnimation:UITableViewRowAnimationFade];
    }   
    else if (editingStyle == UITableViewCellEditingStyleInsert) {
        // Create a new instance of the appropriate class, insert it into the array, and add a new row to the table view
    }   
}
*/

/*
// Override to support rearranging the table view.
- (void)tableView:(UITableView *)tableView moveRowAtIndexPath:(NSIndexPath *)fromIndexPath toIndexPath:(NSIndexPath *)toIndexPath
{
}
*/

/*
// Override to support conditional rearranging of the table view.
- (BOOL)tableView:(UITableView *)tableView canMoveRowAtIndexPath:(NSIndexPath *)indexPath
{
    // Return NO if you do not want the item to be re-orderable.
    return YES;
}
*/


#pragma mark - Navigation

// In a story board-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
{
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
    UIViewController* destination = [segue destinationViewController];
    if ([destination respondsToSelector:@selector(setPreViewController:)])
    {
        [destination setValue:self forKey:@"preViewController"];
    }
    if ([destination respondsToSelector:@selector(setSelection:)])
    {
        NSIndexPath* indexPath = [self.tableView indexPathForCell:sender];
        id object = [self.listArray objectAtIndex:indexPath.row];
        NSDictionary* selection = [NSDictionary dictionaryWithObjectsAndKeys:
                                   indexPath,@"indexPath",
                                   object,@"object",
                                   nil];
        [destination setValue:selection forKey:@"selection"];
    }
}



@end
