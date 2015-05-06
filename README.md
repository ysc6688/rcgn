# rcgn
 Identify simple CAPTCHA by using OpenCV lib. Include pre-treatment of picture and identification by SVM
 If you want to know more about how to identify CAPTCHA, please see the introduction of this project ----   http://blog.csdn.net/ysc6688/article/category/2913009

First please make sure that OpenCV (version 2.4.9 or higher) is installed and the PATH has been configured.

Then compile the source code:
./make

Execution file is generated in ./bin, its name is verification_rcgn
cd ./bin

To generate characteristic value file, you should mark the Captcha and use the characters as its file name. For example, if the characters of a Captcha is AE86, the file name should be AE86.jpg or AE86.png etc. There is no doubt that the marked pictures is not only one. So it is necessary to store all marked files in a folder.
Your can use paragram --data to generate character dataset file.
./verification_rcgn --date Marked_Picture_Forder

Then a file in which each line is a character with its character value will be generated. You can use paragram --train to train the dataset and get XML file like:
./verification_rcgn --train Character_file

At last there will be a new XML file, it is the template file when verificating the Captcha. If the file name of your Captcha to be verificated is test.jpg, Use the following command:
./verification_rcgn test.jpg
In this way, the program will divide the Captcha in small pictures which has a letter each. And then the single letter picture will be showed. Finally the program prints the result.

But if you want to get the result directly, you can use paragram -d like:
./verification_rcgn -d test.jpg
This will just tell you result without other action.
