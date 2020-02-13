using System;

namespace CSharpBlackJack {
    internal class QuickRand {
        #region Constructor

        public QuickRand() {
            _x = (ulong) Guid.NewGuid().GetHashCode();
            _y = (ulong) Guid.NewGuid().GetHashCode();
        }

        #endregion

        public ushort Next() {
            var tempX = _y;
            _x ^= _x << 23;
            var tempY = _x ^ _y ^ (_x >> 17) ^ (_y >> 26);

            var _ = (ushort) (tempY + _y);

            _x = tempX;
            _y = tempY;

            return _;
        }

        #region Data Members

        private ulong _x;
        private ulong _y;

        #endregion
    }
}