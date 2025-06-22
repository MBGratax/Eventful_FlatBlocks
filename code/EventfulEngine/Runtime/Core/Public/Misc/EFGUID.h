#pragma once
#include <cassert>

#include "CoreTypes.h"

namespace EventfulEngine{

    struct EFGUID{
    public:
        /** Default invalid constructor. */
        constexpr EFGUID()
            : A(0)
              , B(0)
              , C(0)
              , D(0){
        }

        /**
         * Creates and initializes a new GUID from the specified components.
         *
         * @param InA The first component.
         * @param InB The second component.
         * @param InC The third component.
         * @param InD The fourth component.
         */
        explicit constexpr EFGUID(const uint32 InA, const uint32 InB, const uint32 InC, const uint32 InD)
            : A(InA), B(InB), C(InC), D(InD){
        }

    public:
        /**
         * Compares two GUIDs for equality.
         *
         * @param X The first GUID to compare.
         * @param Y The second GUID to compare.
         * @return true if the GUIDs are equal, false otherwise.
         */
        friend bool operator==(const EFGUID& X, const EFGUID& Y){
            return ((X.A ^ Y.A) | (X.B ^ Y.B) | (X.C ^ Y.C) | (X.D ^ Y.D)) == 0;
        }

        /**
         * Compares two GUIDs for inequality.
         *
         * @param X The first GUID to compare.
         * @param Y The second GUID to compare.
         * @return true if the GUIDs are not equal, false otherwise.
         */
        friend bool operator!=(const EFGUID& X, const EFGUID& Y){
            return ((X.A ^ Y.A) | (X.B ^ Y.B) | (X.C ^ Y.C) | (X.D ^ Y.D)) != 0;
        }

        /**
         * Compares two GUIDs.
         *
         * @param X The first GUID to compare.
         * @param Y The second GUID to compare.
         * @return true if the first GUID is less than the second one.
         */
        friend bool operator<(const EFGUID& X, const EFGUID& Y){
            if (X.A < Y.A){
                return true;
            }
            if (X.A > Y.A){
                return false;
            }
            if (X.B < Y.B){
                return true;
            }
            if (X.B > Y.B){
                return false;
            }
            if (X.C < Y.C){
                return true;
            }
            if (X.C > Y.C){
                return false;
            }
            if (X.D < Y.D){
                return true;
            }
            if (X.D > Y.D){
                return false;
            }
            return false;
        }

        /**
         * Provides access to the GUIDs components.
         *
         * @param Index The index of the component to return (0...3).
         * @return The component.
         */
        uint32& operator[](const int32 Index){
            assert(Index >= 0 && Index < 4);

            switch (Index){
            case 0: return A;
            case 1: return B;
            case 2: return C;
            case 3: return D;
            default: return A;
            }
        }

        /**
         * Provides read-only access to the GUIDs components.
         *
         * @param Index The index of the component to return (0...3).
         * @return The component.
         */
        const uint32& operator[](const int32 Index) const{
            assert(Index >= 0 && Index < 4);

            switch (Index){
            case 0: return A;
            case 1: return B;
            case 2: return C;
            case 3: return D;
            default: return A;
            }
        }

    public:
        /**
         * Invalidates the GUID.
         *
         * @see IsValid
         */
        void Invalidate(){
            A = B = C = D = 0;
        }

        /**
         * Checks whether this GUID is valid or not.
         *
         * A GUID that has all its components set to zero is considered invalid.
         *
         * @return true if valid, false otherwise.
         * @see Invalidate
         */
        [[nodiscard]] bool IsValid() const{
            return ((A | B | C | D) != 0);
        }

        //TODO: Construct a valid GUID
        static EFCORE_API EFGUID NewGuid();

        /** Holds the first component. */
        uint32 A;

        /** Holds the second component. */
        uint32 B;

        /** Holds the third component. */
        uint32 C;

        /** Holds the fourth component. */
        uint32 D;
    };

} // EventfulEngine
