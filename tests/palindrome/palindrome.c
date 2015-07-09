typedef int bool;
#define true 1
#define false 0

bool palindrome(int n)
{
   int reverse = 0, temp;
   temp = n;
    int a1;
    int a2;

   while( temp != 0 )
   {
      reverse = reverse * 10;
      reverse = reverse + temp%10;
      temp = temp/10;
   }

   if ( n == reverse )
        return true;
   else
        return false;
}
