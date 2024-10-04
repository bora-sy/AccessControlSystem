namespace ACSBackend
{
    public static class Extensions
    {
        public static bool IsNumeric(this string str)
        {
            foreach (char c in str)
            {
                if (!char.IsDigit(c))
                    return false;
            }

            return true;
        }
    }
}
