/** \file closure.h
  *
  * \brief Defines closures, objects that bind functions to creating and calling
  * contexts
  */

#ifndef CLOSURE_H
#define	CLOSURE_H

/** \brief Defines a closure, a tuple <function, context, destructor>
  *
  * A closure is a data structure that binds some function to a predetermined
  * context. When the function is run, this context will be available to
  * the function body.
  *
  * When invoking the closure, additional parameters may be passed along and will
  * also be available to the function body. The function can also return a value
  * that will be passed back by the `closure_invoke` function.
  *
  * An optional destructor may also be attached. If it is, it will be called on
  * closure destruction by the `closure_destroy` function.
  */
typedef struct closure closure_t;
struct closure{
    //! \brief The function to be run when the closure is invoked.
    //! Must task a reference to the closure as param e return a void pointer.
    void * (* function)(closure_t *);
    //! \brief The context supplied to the closure durint creation.
    //! This context will be available during the function execution.
    void *context;
    /** \brief The closure destructor that will be run on `closure_destroy`.
      * If your closure context needs tearing down, attach a custom destructor.
      * It should be responsible by freeing any memory allocated from the heap,
      * if any, or returning dynamic objects contained in the context to their
      * respective pools.
      */
    void (* destructor)(closure_t *);
    //! The parameters supplied by the closure invokation.
    void *params;
    //! The value returned by the last closure invokation.
    void *value;
};

/** \brief Creates a new closure.
  *
  * Binds the tuple <function, context, destructor> and populates a new closure
  * object with it. The object is returned by value.
  * \param function The function to be run on closure invokation.
  * \param context The creation context of the closure.
  * \param destructor The custom destructor to be run on `closure_destroy`.
  * \return The closure object, by value.
  */
closure_t closure_create(
    void * (*function)(closure_t *),
    void *context,
    void (*destructor)(closure_t *)
);

/** \brief Invokes a closure and returns whatever value it returned.
  *
  * \param closure The closure reference to be invoked.
  * \param params The parameters to be passed along during closure invokation.
  * \return This function returns whatever the closure function returned.
  */
void *closure_invoke(closure_t *closure, void *params);

/** \brief Tears a closure down, calling its associated destructor
  *
  * \param closure The closure to be destroyed
  */
void closure_destroy(closure_t *closure);

#endif	/* CLOSURE_H */
