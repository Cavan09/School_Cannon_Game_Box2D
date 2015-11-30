//
//  GameViewController.h
//  GameDevFramework
//
//  Created by Bradley Flood on 12-08-30.
//  Copyright (c) 2012 Algonquin College. All rights reserved.
//

@class CADisplayLink;
@class EAGLContext;
@class OpenGLView;

@interface GameViewController : UIViewController
{
  CADisplayLink *m_DisplayLink;
  EAGLContext *m_OpenGLContext;
  OpenGLView *m_OpenGLView;
  
  UILabel* m_BallsFired;
  UILabel* m_Temperature;
  UILabel* m_Boom;
}

- (void)invalidateRunLoop;
- (IBAction)fireAction:(id)sender;

@property (nonatomic, retain) IBOutlet OpenGLView *openGLView;
@property (nonatomic, retain) IBOutlet UILabel* ballsFired;
@property (nonatomic, retain) IBOutlet UILabel* temperature;
@property (nonatomic, retain) IBOutlet UILabel* boom;

@end

