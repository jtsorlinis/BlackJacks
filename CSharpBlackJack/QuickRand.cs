using System;


namespace NETCoreBlackJack
{
    class QuickRand
    {
        #region Data Members

        private ulong x_;
        private ulong y_;

        #endregion

        #region Constructor

        public QuickRand()
        {
            x_ = (ulong)Guid.NewGuid().GetHashCode();
            y_ = (ulong)Guid.NewGuid().GetHashCode();
        }

        #endregion

        public ushort Next()
        {
            ushort _;
            ulong temp_x, temp_y;

            temp_x = y_;
            x_ ^= x_ << 23; temp_y = x_ ^ y_ ^ (x_ >> 17) ^ (y_ >> 26);

            _ = (ushort)(temp_y + y_);

            x_ = temp_x;
            y_ = temp_y;

            return _;
        }
    }
}
