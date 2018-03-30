use mruby_sys::*;
use std::error::Error;
use std::fmt;
use std::ptr::NonNull;

pub type MrbResult<T> = Result<T, MrbError>;

#[derive(Clone, Debug)]
pub struct MrbError {
    stack: Vec<Inner>,
}

impl MrbError {
    pub fn chain<S: AsRef<str>>(mut self, s: S) -> Self {
        let inner = ErrorKind::Chained.into_inner_cxt(s);
        self.stack.push(inner);
        self
    }
}

impl fmt::Display for MrbError {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        for (i, ref inner) in self.stack.iter().enumerate() {
            write!(f, "{:3}: ", i)?;
            fmt::Display::fmt(inner, f)?;
        }
        Ok(())
    }
}

impl Error for MrbError {
    fn description(&self) -> &str {
        self.stack[0].kind.as_str()
    }
}

impl From<ErrorKind> for MrbError {
    fn from(e: ErrorKind) -> MrbError {
        let inner = e.into_inner();
        MrbError { stack: vec![inner] }
    }
}

macro_rules! chain {
    ($e:expr, $msg:expr,) => {
        chain!($e, $msg)
    };
    ($e:expr, $msg:expr) => {{
        match $e {
            Ok(t) => t,
            Err(e) => return Err(e.chain($msg)),
        }
    }};
    ($e:expr, $fmt:expr, $($arg:tt)+) => {{
        match $e {
            Ok(t) => t,
            Err(e) => return Err(e.chain(format!($fmt, $($arg)+))),
        }
    }};
}

macro_rules! nonnull {
    ($ptr:expr) => {
        if let Some(p) = NonNull::new($ptr) {
            p
        } else {
            return Err(ErrorKind::Null.into());
        }
    };
    ($ptr:expr,) => {
        nonnull!($ptr)
    };
    ($ptr:expr, $msg:expr) => {
        if let Some(p) = NonNull::new($ptr) {
            p
        } else {
            return Err(ErrorKind::Null.into_with($msg));
        }
    };
    ($ptr:expr, $fmt:expr, $($arg:tt)+) => {
        if let Some(p) = NonNull::new($ptr) {
            p
        } else {
            return Err(ErrorKind::Null.into_with(format!($fmt, $($arg)+)));
        }
    };
}

macro_rules! get_ref {
    ($ptr:expr) => {
        if let Some(r) = $ptr.as_mut() {
            r
        } else {
            return Err(ErrorKind::Null.into());
        }
    };
    ($ptr:expr,) => {
        get_ref!($ptr);
    };
    ($ptr:expr, $msg:expr) => {
        if let Some(r) = $ptr.as_mut() {
            r
        } else {
            return Err(ErrorKind::Null.into_with($msg));
        }
    };
    ($ptr:expr, $fmt:expr, $($arg:tt)+) => {
         if let Some(r) = $ptr.as_mut() {
            r
        } else {
            return Err(ErrorKind::Null.into_with(format!($fmt, $($arg)+)));
        }
    };
}

#[derive(Clone, Debug)]
struct Inner {
    kind: ErrorKind,
    context: String,
}

impl fmt::Display for Inner {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        writeln!(f, "kind: {}", self.kind.as_str())?;
        writeln!(f, "    context: {}", self.context)
    }
}

#[derive(Clone, Debug, Eq, PartialEq)]
pub enum ErrorKind {
    FreeVal,
    Null,
    Undefined,
    Chained,
}

impl ErrorKind {
    fn as_str(&self) -> &str {
        use self::ErrorKind::*;
        match *self {
            FreeVal => "FreeVal(invalid access to already freed variable)",
            Null => "Null(access to null pointer)",
            Undefined => "Undefined(access to undefined value or methods)",
            Chained => "Chained(an error occurs in callee)",
        }
    }
    fn into_inner(self) -> Inner {
        Inner {
            kind: self,
            context: String::new(),
        }
    }
    fn into_inner_cxt<S: AsRef<str>>(self, s: S) -> Inner {
        Inner {
            kind: self,
            context: s.as_ref().to_owned(),
        }
    }
    pub fn into_with<S: AsRef<str>>(self, s: S) -> MrbError {
        let inner = self.into_inner_cxt(s);
        MrbError { stack: vec![inner] }
    }
}
