//  Created by Roman Petrov on 21/07/15.
//  Copyright (c) 2015 Kentor. All rights reserved.

import UIKit

class ViewController: UIViewController {
    let videoPath:String = NSBundle.mainBundle().pathForResource("cat", ofType: "mp4")!

    @IBOutlet var videoView:UIImageView!
    @IBOutlet var playStopButton:UIButton!
    var timer:NSTimer!
    var stopped:Bool = true
    var width:Int!
    var height:Int!
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        if !native_video_init(videoPath) {
            handleError("native_video_init error")
            return
        }
        
        var videoRes:[Int32] = [0, 0]
        if !native_get_video_res(&videoRes) {
            handleError("native_get_video_res error")
            return
        }
        width = Int(videoRes[0])
        height = Int(videoRes[1])
        
        if !native_setup_scaler(videoRes[0], videoRes[1]) {
            handleError("native_setup_scaler error")
        }
    }

    @IBAction func buttonClicked(sender: AnyObject) {
        playOrStopVideo()
    }
    
    func playOrStopVideo() {
        if stopped {
            playVideo()
        } else {
            stopVideo()
        }
    }

    func playVideo() {
        // seek to 0.0 seconds
        native_set_seek_time(0.0)
        timer = NSTimer.scheduledTimerWithTimeInterval(1.0 / 30, target: self, selector: Selector("displayNextFrame"), userInfo: nil, repeats: true)
        stopped = false
        setButtonTextStop()
    }
    
    func stopVideo() {
        timer.invalidate()
        stopped = true
        setButtonTextPlay()
    }
    
    func displayNextFrame() {
        if (!native_step_frame()) {
            stopVideo()
        } else {
            videoView.image = getCurrentImage()
        }
    }
    
    func getCurrentImage() -> UIImage {
        let pict = native_get_current_frame()
        let pictData = pict.memory.data
        let pictLinesize = pict.memory.linesize

        let bitmapInfo:CGBitmapInfo = .ByteOrderDefault
        let data:CFData! = CFDataCreateWithBytesNoCopy(kCFAllocatorDefault, pictData.0, Int(pictLinesize.0) * height, kCFAllocatorNull)
        let provider:CGDataProvider = CGDataProviderCreateWithCFData(data)
        let colorSpace:CGColorSpace = CGColorSpaceCreateDeviceRGB()
        let cgImage:CGImage = CGImageCreate(width,
            height,
            8,
            24,
            Int(pictLinesize.0),
            colorSpace,
            bitmapInfo,
            provider,
            nil,
            false,
            kCGRenderingIntentDefault)

        return UIImage(CGImage: cgImage)!
    }
    
    func setButtonTextPlay() {
        setButtonText("Play")
    }
    
    func setButtonTextStop() {
        setButtonText("Stop")
    }
    
    func setButtonText(text:String) {
        playStopButton.setTitle(text, forState: UIControlState.Normal)
    }
    
    func handleError(errorMessage:String) {
        println(errorMessage)
        playStopButton.enabled = false
    }
}

