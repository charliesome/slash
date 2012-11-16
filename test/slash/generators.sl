<%

class TestGenerators extends Test {
    def fib {
        [a, b] = [0, 1];
        while true {
            [a, b] = [b + a, a];
            yield a;
        }
    }
    
    def test_fib {
        assert_equal([1, 1, 2, 3, 5, 8, 13, 21, 34, 55], fib().take(10));
    }
    
    def enumerate {
        yield 1;
        yield 2;
        yield 3;
    }
    
    def test_implicit {
        ary = [];
        for j in self {
            ary.push(j);
        }
        assert_equal(ary, [1, 2, 3]);
    }
    
    def yield_self {
        yield self;
    }
    
    def test_self_remains_self_in_generators {
        assert_is_a(TestGenerators, yield_self().first);
    }
    
    def yield_ivar {
        yield @ivar;
    }
    
    def test_can_access_ivars_in_generators {
        @ivar = "foo";
        assert_equal("foo", yield_ivar().first);
    }
}.register;
