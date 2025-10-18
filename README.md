# JunoCam_GreyscaleByChannelToColor

Simple code program to combine the color chennel greyscale images from JunoCam
into one single image where pixel lightness of each image combines to make a 
single RGB img.

## Some Generated Examples

1. This first photo is from Perijove 73 (September of 2025). It seems that the
  blue light sensor was damaged during JunoCam's 73rd Perijove (closest approach
  to Jupiter) flyby. This caused an imense amount of noise/interference that 
  rendered the blue light data completely useless. The effects of this noisy 
  data manifests in the form of green static/noise in the generated composite 
  image, shown below:

![Perijove 73 \(PJ73\) generated composite photo of Jupiter's Northern Folded Filamentary Region](/generated_sample_imgs/JNCE_2025159_73C00025_V01-output.png)

2. Curious to see how my code would look with earlier flyby data, I decided to
  try photos from much earlier in the Juno mission's lifetime (Perijove 27 for
  the generated image shown below). I think the lack of any green interference
  static suffices to say that trying photos from earlier flyby's yielded great
  results!

![Perjove 28 \(PJ27\) generated composite photo of Jupiter's Southern Circumpolar Cyclones](/generated_sample_imgs/JNCE_2020154_27C00057_V01-output.png)

