#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
#include "il2cpp.hpp"
#include "hooks.hpp"
#include "menu.hpp"

@interface AstralOverlay : UIView
@end

@implementation AstralOverlay
- (void)drawRect:(CGRect)rect {
    // optional: CoreGraphics render of Menu::Commands()
    // for production use Metal layer or ImGui
}
- (void)touchesBegan:(NSSet<UITouch*>*)touches withEvent:(UIEvent*)event {
    UITouch* t = touches.anyObject;
    CGPoint p = [t locationInView:self];
    Menu::OnTouch(p.x, p.y, true, false);
}
- (void)touchesMoved:(NSSet<UITouch*>*)touches withEvent:(UIEvent*)event {
    UITouch* t = touches.anyObject;
    CGPoint p = [t locationInView:self];
    Menu::OnTouch(p.x, p.y, true, false);
}
- (void)touchesEnded:(NSSet<UITouch*>*)touches withEvent:(UIEvent*)event {
    UITouch* t = touches.anyObject;
    CGPoint p = [t locationInView:self];
    Menu::OnTouch(p.x, p.y, false, true);
}
- (void)touchesCancelled:(NSSet<UITouch*>*)touches withEvent:(UIEvent*)event {
    Menu::OnTouch(-1, -1, false, true);
}
@end

static AstralOverlay* g_overlay = nil;
static CADisplayLink* g_link = nil;

static void OnDisplayLink(CADisplayLink* link) {
    CGSize sz = [UIScreen mainScreen].bounds.size;
    Hooks::OnFrame(sz.width, sz.height);
    [g_overlay setNeedsDisplay];
}

__attribute__((constructor))
static void AstralInit() {
    Il2CppInit();
    Hooks::Install();

    dispatch_async(dispatch_get_main_queue(), ^{
        UIWindow* win = [UIApplication sharedApplication].keyWindow;
        if (!win) {
            // retry next runloop
            dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(1.0 * NSEC_PER_SEC)),
                           dispatch_get_main_queue(), ^{ AstralInit(); });
            return;
        }
        g_overlay = [[AstralOverlay alloc] initWithFrame:win.bounds];
        g_overlay.backgroundColor = [UIColor clearColor];
        g_overlay.userInteractionEnabled = YES;
        g_overlay.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
        [win addSubview:g_overlay];
        [win bringSubviewToFront:g_overlay];

        g_link = [CADisplayLink displayLinkWithTarget:[NSObject new]
                                             selector:@selector(doesNotRecognizeSelector:)];
        // use block-based via proxy
        g_link = [CADisplayLink displayLinkWithTarget:NSClassFromString(@"NSObject")
                                             selector:nil];
        // simpler: timer
        [NSTimer scheduledTimerWithTimeInterval:1.0/60.0
                                        repeats:YES
                                          block:^(NSTimer* t) {
            CGSize sz = [UIScreen mainScreen].bounds.size;
            Hooks::OnFrame(sz.width, sz.height);
            [g_overlay setNeedsDisplay];
        }];
    });
}
